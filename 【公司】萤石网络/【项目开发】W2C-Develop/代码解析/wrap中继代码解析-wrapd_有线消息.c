
/*
1、wrap_main_function为线程函数。
2、通过检查桥口br-lan上是否有新的接口加入或退出，来创建和删除proxy vap。
*/
void *wrap_main_function()
{
    int i,n,offset=0;
    struct new_fdb_table *new_table=NULL, *temp = NULL;
    table=(struct fdb_table*)os_malloc(MAX_VAP_LIMIT * sizeof(struct fdb_table));
    data->ioctl_sock=0;
    if((data->ioctl_sock=socket(AF_LOCAL, SOCK_STREAM, 0))<0)
    {
    	wrapd_printf("error in ioctl socket opening");
        goto out;
    }
    data->table_no=0;
    while(1)
    {
	wrap_check_success_flag();
	offset=0;
	data->new_table_no=0;
	for(i=0;i<data->no_of_interfaces;i++)
	{
            data->port[i]=wrap_interface_to_port(data->ifname[i]);
	}
	while(1)
	{
	    temp = realloc(new_table, (offset+CHUNK) * sizeof
						(struct new_fdb_table));
	    if(!temp)
            {
                wrapd_printf("out of memory");
                break;
            }
            new_table = temp;
			/* 读取桥上端口fdb*/
            n=wrap_read_forwarding_database(new_table+offset, offset, CHUNK);
            if(n==0)
	    {
                break;
	    }
            offset+=n;
	}
	if(data->table_no==0 && data->new_table_no!=0)
	{
	    for(i=0; i < data->new_table_no; i++)
	    {
		struct new_fdb_table *entry=NULL;
		if(data->table_no<data->vap_limit && global_vap_limit_flag==0)
		{
		    entry=wrap_find_minimum_ageing_time
					(new_table, data->new_table_no);
		    if(entry!=NULL)
		    {
			/*有新网络接口加入到桥上，创建vap*/
			wrap_create_vap(entry);
		    }
		}
	    }
	}
	else if(data->table_no!=0 && data->new_table_no==0
					&& data->delete_enable==1)
	{
	    for(i=0;i<data->table_no;i++)
	    {
		struct fdb_table *entry=table+i;
		if(entry->success_flag==0)
		{
			/*有网络接口退出桥，删除vap*/
		    wrap_delete_vap(entry);
		}
	    }
	}
	else if(data->table_no != 0 && data->new_table_no != 0)
	{
	    wrap_compare_mac_entries(new_table);
	    if(data->delete_enable==1)
	    {
		for(i=0;i<data->table_no;i++)
		{
		    struct fdb_table *entry=table+i;
		    if(entry->flag==0 && entry->success_flag==0)
		    {
			wrap_delete_vap(entry);
		    }
		}
	    }
	    for(i=0;i<data->new_table_no;i++)
            {
		struct new_fdb_table *entry=NULL;
		if(data->table_no<data->vap_limit && global_vap_limit_flag==0)
		{
                    entry=wrap_find_minimum_ageing_time
					(new_table, data->new_table_no);
                    if(entry!=NULL)
		    {
                        wrap_create_vap(entry);
		    }
		}
            }
	    for(i=0;i<data->table_no;i++)
	    {
		(table+i)->flag=0;
	    }
	}
	sleep(data->sleep_timer);
    }

out:
    wrapd_printf("ERROR: Exiting thread %s\n", __func__);
    pthread_exit(NULL);
}

/*
通过sys文件系统，读取桥上端口信息
*/
int wrap_read_forwarding_database(struct new_fdb_table *fdbs, unsigned long offset, int chunk)
{
    FILE *fp;
    int i,j=0,k,n=0;
    struct __fdb_entry entry[chunk];
    char path[SYSFS_PATH_MAX];
    unsigned long num_entries = MAX_READ_LENGTH/sizeof(struct __fdb_entry);
    unsigned short read_complete = 0;
    unsigned short read_progress = 0;
    snprintf(path, SYSFS_PATH_MAX, SYS_CLASS_NET "%s/SYSFS_BRIDGE_FDB", data->brname);
    fp=fopen(path,"r");
    if(fp)
    {
        do{
            if((chunk-read_complete) > num_entries)
	    {
                read_progress += num_entries;
            }
            else
            {
                read_progress += chunk - read_complete;
            }
            fp=fopen(path,"r");
            if(fp)
            {
                fseek(fp, (read_complete + offset) * sizeof(struct __fdb_entry), SEEK_SET);
                n=fread(entry, sizeof(struct __fdb_entry), num_entries, fp);
		read_complete += n;
                fclose(fp);
            }
	} while(read_complete != chunk && n!=0);
    }
    else if(data->ioctl_sock >= 0)
    {
	int retry = 0;
retry:
	    read_complete = br_dev_ioctl(data, BRCTL_GET_FDB_ENTRIES, (unsigned long)entry, chunk, offset);
            if (read_complete < 0 && errno==EAGAIN && ++retry<10)
            {
                goto retry;
            }
    }
    for(i=0;i<read_complete;i++)
    {
        struct __fdb_entry *from=entry + i;
        for(k=0; k < data->no_of_interfaces;k++)
        {
            if( (from->port_no==data->port[k]) && (from->is_local==0))
            {
                wrap_copy_fdb(fdbs+j, from);
                j++;
            }
        }
    }
    data->new_table_no+=j;
    return read_complete;
}

/*
1、此函数只为有线口创建vap
2、组建消息报文 “ETH_PSTA_ADD mac地址”通过unix套接字wrapd-wifi0发送。
*/
/*
Issues a socket message to wrapd global listener for creation of vap
and adds the entry at the last of the Vap table.
*/
static void wrap_create_vap(struct new_fdb_table *entry)
{
    char msg[1024],mac[100];
    snprintf(mac,100,"%.2x-%.2x-%.2x-%.2x-%.2x-%.2x",entry->mac_addr[0],
		entry->mac_addr[1], entry->mac_addr[2],entry->mac_addr[3],
		entry->mac_addr[4], entry->mac_addr[5]);
    snprintf(msg, sizeof(msg),"ETH_PSTA_ADD %s", mac);
    struct fdb_table *new_entry=table + data->table_no;
    os_memcpy(new_entry->mac_addr, entry->mac_addr, IEEE80211_ADDR_LEN);
    new_entry->flag=0;
    new_entry->success_flag=1;
    data->table_no++;
    wrapd_send_msg(msg, 128, wrapg->wrapd_ctrl_intf);
}

/*
1、wrapd_ctrl_iface_receive为unix套接字wrapd_wifi0的回调函数，
2、通过eloop触发接收报文消息，
*/
void
wrapd_ctrl_iface_receive(int sock, void *eloop_ctx, void *sock_ctx)
{
    struct wrap_wifi_iface *aptr = (struct wrap_wifi_iface *)eloop_ctx;
    char buf[256];
    int res;
    struct sockaddr_un from;
    socklen_t fromlen = sizeof(from);
    char *reply;
    size_t reply_len;
    res = recvfrom(sock, buf, sizeof(buf) - 1, 0, (struct sockaddr *) &from, &fromlen);
    if (res < 0) {
        wrapd_printf("recvfrom err");
        return;
    }
    buf[res] = '\0';

	/* pengruofeng debug */
	wrapd_printf("[pengruofeng][wrapd-wifi0] recv buf (%s) \n",buf);
	reply = wrapd_ctrl_iface_process(aptr, buf, &reply_len);
	if (reply) {
		wrapd_printf("[pengruofeng][wrapd-wifi0] send buf (%s) \n",reply);
    }


    if (reply) {
        sendto(sock, reply, reply_len, 0, (struct sockaddr *) &from, fromlen);
        os_free(reply);
    } else if (reply_len) {
        sendto(sock, "FAIL\n", 5, 0, (struct sockaddr *) &from, fromlen);
    }
}

/*
1、如果消息中包含ETH_PSTA_ADD，使用wrapd_psta_add来创建proxy vap
*/
char *
wrapd_ctrl_iface_process(struct wrap_wifi_iface *aptr, char *buf, size_t *resp_len)
{
	char *reply;
	const int reply_size = 2048;
	int reply_len, addr_off;
    u_int32_t flags = 0;
    struct wrap_wifi_iface *dev;

	reply = os_malloc(reply_size);
	if (reply == NULL) {
		*resp_len = 1;
		return NULL;
	}

	os_memcpy(reply, "OK\n", 3);
	reply_len = 3;

	if (os_strcmp(buf, "PING") == 0) {
		os_memcpy(reply, "PONG\n", 5);
		reply_len = 5;

	} else if (os_strncmp(buf, "ETH_PSTA_ADD ", 13) == 0) {
        if (os_strncmp(buf + 13, "MAT ", 4) == 0) {
            flags |= (WRAPD_PSTA_FLAG_MAT | WRAPD_PSTA_FLAG_WIRED);
            addr_off = 13 + 4;

        } else {
            flags |= WRAPD_PSTA_FLAG_WIRED;
            addr_off = 13;
        }
        if (char2addr(buf + addr_off) != 0) {
            wrapd_printf("Invalid MAC addr");
            return NULL;
        }
        if(wrapg->always_primary)
        {
            for(dev = wrapg->wrap_wifi_list; dev; dev=dev->next)
            {
                wrapd_psta_add(dev, dev->wifi_ifname, NULL, (u8 *)(buf + addr_off), flags);
            }
        }
        else
        {
            /* Find primary radio and send add command to the same */
	    	wrapd_psta_add(aptr, aptr->wifi_ifname, NULL, (u8 *)(buf + addr_off), flags);
        }

	} else if (os_strncmp(buf, "ETH_PSTA_REMOVE ", 16) == 0) {
        if (char2addr(buf + 16) != 0) {
            wrapd_printf("Invalid MAC addr");
            return NULL;
        }
        if(wrapg->always_primary)
        {
            for(dev = wrapg->wrap_wifi_list; dev; dev=dev->next)
            {
		        wrapd_psta_remove(dev, (u8 *)(buf + 16), 1);
            }
        }
        else
        {
		    wrapd_psta_remove(aptr, (u8 *)(buf + 16), 1);
        }

	} else if (os_strcmp(buf, "PSTA_LIST") == 0) {
		wrapd_sta_list(aptr);
    } else {
		os_memcpy(reply, "UNKNOWN COMMAND\n", 16);
		reply_len = 16;
	}

	if (reply_len < 0) {
		os_memcpy(reply, "FAIL\n", 5);
		reply_len = 5;
	}

	*resp_len = reply_len;
	return reply;
}
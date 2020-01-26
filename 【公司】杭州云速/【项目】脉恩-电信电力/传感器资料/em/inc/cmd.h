#ifndef __CMD_H__
#define __CMD_H__

#define COMMAND_PACKAGE_PREFIX1             0x80
#define COMMAND_PACKAGE_SET                 0x01
#define COMMAND_PACKAGE_RW                  0x03
#define COMMAND_PACKAGE_SUFIX1              0x11
#define COMMAND_PACKAGE_SUFIX2              0x22
#define COMMAND_PACKAGE_SUFIX3              0x33
#define COMMAND_PACKAGE_SUFIX4              0x44

#define COMMAND_PACKAGE_FUNCTION_EPUI      0x1000
#define COMMAND_PACKAGE_FUNCTION_ENERGY    0x1000
#define COMMAND_PACKAGE_FUNCTION_PEYMDHM   0x1001
#define COMMAND_PACKAGE_FUNCTION_XYZ       0x1010
#define COMMAND_PACKAGE_FUNCTION_YDHMS     0x1002
#define COMMAND_PACKAGE_FUNCTION_MODE      0x1012
#define COMMAND_PACKAGE_FUNCTION_SECURITY  0x1015
#define COMMAND_PACKAGE_FUNCTION_BROADCAST 0x1011
#define COMMAND_PACKAGE_FUNCTION_ID        0x10AA
#define COMMAND_PACKAGE_FUNCTION_VERSION        0x10AB
#define COMMAND_PACKAGE_FUNCTION_TM        0x1021
#define COMMAND_PACKAGE_FUNCTION_EPUI_PULSE      0x1022


#define COMMAND_FLAGS_EPUI                  0x01    /*��ȡ�������ʵ�ѹ����*/
#define COMMAND_FLAGS_PEYMDHM               0x02    /*��ȡ��Ӧʱ��㹦������*/
#define COMMAND_FLAGS_YDHMS                 0x03    /*��ȡʱ��*/
#define COMMAND_FLAGS_XYZ                   0x04    /*��ȡ��������*/
#define COMMAND_FLAGS_ERR                   0x05    /*�������*/
#define COMMAND_FLAGS_SET                   0x06    /*��������*/
#define COMMAND_FLAGS_SEND_BROADCAST        0x07    /*֪ͨ*/
#define COMMAND_FLAGS_ID                    0x08    /*����ID*/
#define COMMAND_FLAGS_TM                    0x09    /*��ȡ�¶�+����*/
#define COMMAND_FLAGS_EPUI_PULSE            0x0A    /*��ȡ�������ʵ�ѹ��������*/
#define COMMAND_FLAGS_VERSION               0x0b    /*��ȡVER*/



#endif
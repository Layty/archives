@echo on

if exist C:\Program Files (x86)\Qualcomm\QDART\bin cd C:\Program Files (x86)\Qualcomm\QDART\bin
ATTRIB -R *.dll
DEL *.dll
cd/
if exist C:\Qualcomm\WCN\ProdTests\BIN cd C:\Qualcomm\WCN\ProdTests\BIN
ATTRIB -R *.dll
DEL *.dll



#pragma once
#define DLL_EXPORT extern "C" __declspec(dllexport)
/*
* 本模块用于x64程序
* 本模块可实现AES和RSA加解密，可以截取桌面图片，但图片类型只能是bmp
* 也可以计算32位MD5
*/

#define AES_KEY_IV_SIZE 16
#define ALIGN_AES_SIZE(size) (size% AES_KEY_IV_SIZE ? \
        size / AES_KEY_IV_SIZE + 1 : size / AES_KEY_IV_SIZE)* AES_KEY_IV_SIZE


/*
   类型声明
*/
typedef const unsigned char* PCUCHAR;
typedef unsigned char* PUCHAR;
typedef unsigned char UCHAR;

/*
	导出函数声明
*/

/*
	作用：计算数据的MD5值
	参数：
		参数1：要计算MD5的数据地址
		参数2：存储MD5值的缓冲区地址
	返回值：空
	备  注：空
*/
DLL_EXPORT void GetMD5(const char* data, char* out32);


/*
	作用：AES加密
	参数：
		参数1：被加密数据地址
		参数2：数据字节大小,要加密数据对齐之后的值
		参数3：存储加密后的密文缓冲区,大小是参数2按照16对齐后的值
	返回值：成功返回真，否则假
	备  注：AES加密方式是CBC模式
*/
DLL_EXPORT bool AESEncrpty(PCUCHAR pData, size_t dataSize, PUCHAR pOut);


/*
	作用：AES解密
	参数：
		参数1：被解密数据地址
		参数2：数据字节大小
		参数3：存储解密后的数据缓冲区,大小是参数2按照16对齐后的值
	返回值：成功返回真，否则假
	备  注：AES解密方式是CBC模式
*/
DLL_EXPORT bool AESDecrpty(PCUCHAR pData, size_t dataSize, PUCHAR pOut);


/*
	作用：AES加密
	参数：
		参数1：被加密文件路径
		参数2：加密后的新文件路径
	返回值：成功返回真，否则假
	备  注：AES加密方式是CBC模式
*/
DLL_EXPORT bool AESEncrptyFile(char* inPath, char* outPath);


/*
	作用：AES解密
	参数：
		参数1：被解密文件路径
		参数2：解密后的新文件路径
	返回值：成功返回真，否则假
	备  注：AES解密方式是CBC模式
*/
DLL_EXPORT bool AESDecrptyFile(char* inPath, char* outPath);


/*
	作用：初始化AES密钥
	参数：无
	返回值：空
	备  注：使用AES加解密时，必须先调用初始化函数
*/
DLL_EXPORT void InitAES();


/*
	作用：初始化AES密钥
	参数：
		参数1：16位密钥
		参数2：16位IV
	返回值：空
	备  注：使用AES加解密时，必须先调用初始化函数
*/
DLL_EXPORT void SetAESKeyAndIv(UCHAR* pKey, UCHAR* pIv);


/*
	作用：获取AES密钥值
	参数：无
	返回值：AES密钥地址
	备  注：AES解密方式是CBC模式
*/
DLL_EXPORT PUCHAR GetAESKey();


/*
	作用：获取AES的IV值
	参数：无
	返回值：AES的IV地址
	备  注：AES解密方式是CBC模式
*/
DLL_EXPORT PUCHAR GetAESIv();


/*
	作用：初始化RSA密钥
	参数：无
	返回值：空
	备  注：使用RSA加解密时速度很慢，加密数据不宜超过100字节
*/
DLL_EXPORT bool InitRSA();


/*
	作用：释放RSA占用的内存
	参数：无
	返回值：空
	备  注：不用RSA的时候，一定要调用此函数，不然会造成内存泄露
			另外，如果没调用InitRSA()，就不需要调用此函数
*/
DLL_EXPORT void ReleaseRSA();


/*
	作用：获取RSA私钥
	参数：
		参数1：存储私钥的缓冲区地址
	返回值：私钥的字节大小
	备  注：参数是个二级指针
*/
DLL_EXPORT int GetPriBuf(PUCHAR* pPrivate);


/*
	作用：设置RSA私钥
	参数：
		参数1：存储私钥的缓冲区地址
		参数2：私钥大小
	返回值：空
	备  注：无
*/
DLL_EXPORT void SetPriKey(PUCHAR pPrivate, int nSize);

/*
	作用：获取RSA公钥
	参数：
		参数1：存储公钥的缓冲区地址
	返回值：公钥的字节大小
	备  注：参数是个二级指针
*/
DLL_EXPORT int GetPubBuf(PUCHAR* pPublic);


/*
	作用：设置RSA公钥
	参数：
		参数1：存储公钥的缓冲区地址
		参数2：公钥大小
	返回值：空
	备  注：无
*/
DLL_EXPORT void SetPubKey(PUCHAR pPublic, int nSize);


/*
	作用：RSA加密
	参数：
		参数1：被加密数据地址
		参数2：数据字节大小
		参数3：存储加密后的密文字节大小
	返回值：成功返回密文地址，否则返回0
	备  注：无
*/
DLL_EXPORT PUCHAR RSAEncrypt(PUCHAR pBuf, int nBufSize, int* outSize);


/*
	作用：RSA解密
	参数：
		参数1：被解密数据地址
		参数2：数据字节大小
		参数3：存储解密后的明文字节大小
	返回值：成功返回明文地址，否则返回0
	备  注：解密后的密文大小不一定就是原始数据的真实大小
*/
DLL_EXPORT PUCHAR RSADecrypt(PUCHAR pBuf, int nBufSize, int* outSize);


/*
	作用：获取桌面截图
	参数：
		参数1：截图存储路径，默认存储在当前目录中，名字是default.bmp
	返回值：空
	备  注：无
*/
DLL_EXPORT void GetDesktopScreenPic(PCUCHAR pPath = NULL);


/*
	作用：获取桌面截图
	参数：无
	返回值：桌面图片内存地址
	备  注：图片类型是bmp，不支持其他类型
*/
DLL_EXPORT PUCHAR Screenshot();


/*
	作用：获取桌面截图字节大小
	参数：无
	返回值：桌面图片字节大小
	备  注：图片类型是bmp，不支持其他类型
*/
DLL_EXPORT unsigned int GetScreenPicSize();



/*
	作用：释放截图占用的内存资源
	参数：无
	返回值：空
	备  注：截完图后要释放，不然会造成内存泄露
*/
DLL_EXPORT void ReleasePic();









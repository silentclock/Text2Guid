#ifndef __SUPPORT_MD5_H__
#define __SUPPORT_MD5_H__

#include <string>

std::string GetMD5(const void *buf, size_t bufLen);
std::string GetMD5Guid(const void *buf, size_t bufLen);

#endif
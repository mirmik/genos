#ifndef IRCC_H_
#define IRCC_H_

#ifdef __cplusplus
#include <stdlib.h>
#include <string>
#include <vector>
extern std::string ircc_string(const std::string &key);
extern std::vector<uint8_t> ircc_vector(const std::string &key);
extern std::pair<const char *, size_t> ircc_pair(const std::string &key);
extern std::vector<std::string> ircc_keys();
extern "C" const char *ircc_c_string(const char *key, size_t *sizeptr);
extern "C" const char *ircc_name_by_no(size_t no);
#else
#include <stdlib.h>
extern const char *ircc_c_string(const char *key, size_t *sizeptr);
const char *ircc_name_by_no(size_t no);
#endif

#endif
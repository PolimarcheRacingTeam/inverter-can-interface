#ifndef ERROR_MAP_H
#define ERROR_MAP_H

#include <unordered_map>

// Dichiarazione della mappa degli errori
extern const std::unordered_map<uint16_t, std::pair<const char*, const char*>> ERROR_MAP;

#endif // ERROR_MAP_H

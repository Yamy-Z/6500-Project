#ifndef DATALOADER_H
#define DATALOADER_H

#include <vector>
#include <string>
#include <cstdint>

std::vector<std::uint64_t> load_data(const std::string& filename);

#endif // DATALOADER_H
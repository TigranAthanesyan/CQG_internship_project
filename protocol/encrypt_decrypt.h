#pragma once
#include "request.h"

std::string Encrypt(const std::vector<TypedWord>&, const std::set<std::string>&);
std::vector<TypedWord> Decrypt(const std::string&, const std::set<std::string>&);
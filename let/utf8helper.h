#pragma once
#include <iostream>
#include <vector>

size_t ucv_dec(std::vector<char>& charvec, size_t index);
size_t ucv_inc(std::vector<char>& charvec, size_t index);

static bool is_utf8_continuation_byte(char byte) {
	return (byte & 0xC0) == 0x80;
}

int ucv_delete_from_next(std::vector<char>& ucv, size_t index /* first idx of the next byte cluster */)
{
	size_t firstIdx = ucv_dec(ucv, index);
	ucv.erase(ucv.begin() + firstIdx, ucv.begin() + index);
	return index - firstIdx;
}

int ucv_delete_from_first(std::vector<char>& ucv, size_t index /* first index of the byte cluster */)
{
	size_t nextIdx = ucv_inc(ucv, index);
	ucv.erase(ucv.begin() + index, ucv.begin() + nextIdx);
	return nextIdx - index;
}

size_t ucv_index_to_count(std::vector<char>& ucv, size_t index)
{
	size_t count = 0;
	for (size_t i = 0; i < index; i = ucv_inc(ucv, i)) count++;
	return count;
}

size_t ucv_count_to_index(std::vector<char>& ucv, size_t count)
{
	size_t index = 0;
	for (size_t i = 0; i < count && index < ucv.size(); i++) index = ucv_inc(ucv, index);
	return index;
}

size_t ucv_dec(std::vector<char>& ucv, size_t index)
{
	if (index <= 0) throw std::runtime_error("can not increase end index of utf8 char vector");
	do index--;
	while (index > 0 && is_utf8_continuation_byte(ucv[index]));
	return index;
}

size_t ucv_inc(std::vector<char>& ucv, size_t index)
{
	if (index >= ucv.size()) throw std::runtime_error("can not increase end index of utf8 char vector");
	do index++;
	while (index < ucv.size() && is_utf8_continuation_byte(ucv[index]));
	return index;
}
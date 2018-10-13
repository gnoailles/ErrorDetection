#include <iomanip>
#include <iostream>
#include <chrono>
#include <array>
#include <random>

#include "Checksums.h"
#include "CRC.h"


int main()
{
	using clock = std::chrono::high_resolution_clock;
	using fms = std::chrono::duration<double, std::milli>;
	fms duration;
	uint32_t result;
	clock::time_point start;
	
	std::array<uint16_t, 100> data{};
	// std::array<unsigned char,9> data {0x31, 0x32, 0x33, 0x34, 0x35, 0x36, 0x37, 0x38, 0x39};
	std::random_device rd;
	std::default_random_engine dre(rd());
	std::uniform_int_distribution<int> uid(0,UINT8_MAX);
	
	std::generate(data.begin(), data.end(), [&] () { return uid(dre); });

#pragma region Adler32

	start		= clock::now();
	result		= Adler32((uint16_t*)data.data(), static_cast<unsigned int>(data.size()));
	duration	= clock::now() - start;
	std::cout << "Adler32 \t\t duration: " << duration.count() << "ms \t result: 0x" << std::hex << std::uppercase << std::setfill('0') << std::setw(8) << result << std::endl;
#pragma endregion

#pragma region Fletcher32

	start		= clock::now();
	result		= Fletcher32((uint16_t*)data.data(), static_cast<unsigned int>(data.size()));
	duration	= clock::now() - start;
	std::cout << "Fletcher32 \t\t duration: " << duration.count() << "ms \t result: 0x" << result << std::endl;
#pragma endregion

#pragma region Improved Fletcher32

	start		= clock::now();
	result		= Fletcher32_improved((uint16_t*)data.data(), static_cast<unsigned int>(data.size()));
	duration	= clock::now() - start;
	std::cout << "Improved Fletcher32 \t duration: " << duration.count() << "ms \t result: 0x" << result << std::endl;
#pragma endregion


// std::cout << "\nThe CRC check value for the CRC32 standard is \t 0xCBF43926" << std::endl << std::endl;

#pragma region CRC32 custom
	CRC32::InitTable();
	start		= clock::now();
	result		= CRC32::GetCRCTableBased((unsigned char*)data.data(), data.size() * 2);
	duration	= clock::now() - start;
	std::cout << "CRC32 Custom \t\t duration: " << duration.count() << "ms \t result: 0x" << result << std::endl;
#pragma endregion

	std::cin.get();
}

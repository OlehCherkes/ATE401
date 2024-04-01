#include "pch.h"
#include <cstdint>
#include <cstring>
#include <string>
#include "ate401_uart.h"

TEST(GetPackStartTest, BasicTest) {
  uint8_t data[] = { 0x23, 0x40, 0x21, 0x4, 0xd, 0x1, 0x15 };
  size_t size = sizeof(data);

  auto* result = getPackStart(data, size, MAGIC);

  ASSERT_NE(result, nullptr);

  // check that the bytes pointed to by result correspond to the beginning of the substring
  auto i = 0;
  while (i < MAGIC.size())
  {
    EXPECT_EQ(result[i], MAGIC[i]);
    ++i;
  }

  // testing whether crc matched
  uint8_t len = result[MAGIC.size()] - 1; // -1 len without crc
  uint8_t crc = calculateCRC8(&result[0], MAGIC.size() + len);

  EXPECT_EQ(result[MAGIC.size() + len], crc); 
}

// stacked packages
TEST(GetPackStartTest, StackedPackages) {
  uint8_t data[] = { 0x23, 0x40, 0x21, 0x4, 0xd, 0x23, 0x40, 0x21, 0x4, 0xd, 0x1, 0x15 };
  size_t size = sizeof(data);

  auto* result = getPackStart(data, size, MAGIC);

  ASSERT_NE(result, nullptr);

  // check that the bytes pointed to by result correspond to the beginning of the substring
  auto i = 0;
  while (i < MAGIC.size())
  {
    EXPECT_EQ(result[i], MAGIC[i]);
    ++i;
  }

  // testing whether crc matched
  uint8_t len = result[MAGIC.size()] - 1; // -1 len without crc
  uint8_t crc = calculateCRC8(&result[0], MAGIC.size() + len);

  EXPECT_EQ(result[MAGIC.size() + len], crc);
}

// testing the getPackStart function in case the substring is not found
TEST(GetPackStartTest, SubstringNotFoundTest) {

  uint8_t data[] = { 0x23, 0x40, 0x11, 0x21, 0x4, 0xd, 0x1, 0x15 };
  size_t size = sizeof(data);

  auto* result = getPackStart(data, size, MAGIC);

  ASSERT_EQ(result, nullptr);
}

// testing whether crc matched
TEST(GetPackStartTest, CRC) {

  uint8_t data[] = { 0x23, 0x40, 0x21, 0x4, 0xd, 0x1, 0x14 };
  size_t size = sizeof(data);

  auto* result = getPackStart(data, size, MAGIC);

  ASSERT_EQ(result, nullptr);
}

int main(int argc, char** argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
#include <format>
#include <sstream>
#include <string>

#include <gtest/gtest.h>
#include <librepr/object_info.h>

struct MockType {
  unsigned first;
  std::string second;
};

TEST(ObjectInfo, to_string) {
  auto value = MockType{3U, "3"};

  auto info = librepr::ObjectInfo{
    value, 
    librepr::BracedList{
      librepr::ObjectInfo{value.first, librepr::Literal{"3U"}},
      librepr::ObjectInfo{value.second, librepr::OptionalSuffix{"\"3\"", "s"}}}};

  EXPECT_EQ(info.to_string(false, false), "{3U, \"3\"s}");
  EXPECT_EQ(info.to_string(false, true),  "{3U, std::string{\"3\"}}");
  EXPECT_EQ(info.to_string(true, false),  "MockType{3U, \"3\"s}");
  EXPECT_EQ(info.to_string(true, true),   "MockType{3U, std::string{\"3\"}}");
}

TEST(ObjectInfo, output) {
  auto value = 3;
  auto info = librepr::ObjectInfo{value, "3"};

  std::ostringstream stream{};
  stream << info;
  EXPECT_EQ(stream.str(), "3");

  EXPECT_EQ(std::format("{}", info), "3");
}
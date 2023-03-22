#include "parser.hpp"
#include <gtest/gtest.h>

/*

 "0 18446744073709551615 9999999999999999999"
 "18446744073709551616"
 "00 001 0002 00003 00004 0000000000000000000000000000990"
 ""
 " \t \n \v \f \r"
 " "
 "7"
 "!"
 " 1a a1a a1 1a1 2z"
 "      Both  sides  are  spaces      "
 "Эта строка в кодировке utf-8."
 
 */

void setStartState(parseState *state) { 
  *state = start;
}

void setEndState(parseState *state) {
  *state = end;
}

std::string runString(std::string str) {
  return str;
}

uint64_t runDigit(uint64_t num) {
  return num;
}

TEST(TokenParser, NoStartEndCallBack)
{
  TokenParser parser;
  parser.Parse("");
  ASSERT_EQ(parser.getState(), none);
}

TEST(TokenParser, StartCallBack)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.Parse("");
  ASSERT_EQ(parser.getState(), start);
}

TEST(TokenParser, EndCallBack)
{
  TokenParser parser;
  parser.SetEndCallback(&setEndState);
  parser.Parse("");
  ASSERT_EQ(parser.getState(), end);
}

TEST(TokenParser, ValidNumber)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.Parse("0 18446744073709551615 9999999999999999999");
  std::vector <uint64_t> dt = parser.getDigitTokens();

  ASSERT_EQ(dt.size(), 3ul);
  ASSERT_EQ(dt[0], 0ul);
  ASSERT_EQ(dt[1], 18446744073709551615ul);
  ASSERT_EQ(dt[2], 9999999999999999999ul);
}

TEST(TokenParser, InvalidNumber)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("18446744073709551616");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 0ul);
  ASSERT_EQ(st[0], "18446744073709551616");
}

TEST(TokenParser, LeadingZeros)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("00 001 0002 00003 00004 0000000000000000000000000000990");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 6ul);
  ASSERT_EQ(st.size(), 0ul);
  ASSERT_EQ(dt[0], 0ul);
  ASSERT_EQ(dt[1], 1ul);
  ASSERT_EQ(dt[2], 2ul);
  ASSERT_EQ(dt[3], 3ul);
  ASSERT_EQ(dt[4], 4ul);
  ASSERT_EQ(dt[5], 990ul);
}

TEST(TokenParser, EmptyLine)
{
  TokenParser parser;

  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size() + st.size(), 0ul);
}

TEST(TokenParser, LineOfSpaces)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse(" \t \n \v \f \r");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size() + st.size(), 0ul);
}

TEST(TokenParser, SingleSpace)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse(" ");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 0ul);
  ASSERT_EQ(st.size(), 0ul);
}

TEST(TokenParser, SingleDigit)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("7");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 1ul);
  ASSERT_EQ(dt[0], 7ul);
  ASSERT_EQ(st.size(), 0ul);
}

TEST(TokenParser, SingleChar)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("!");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 0ul);
  ASSERT_EQ(st.size(), 1ul);
  ASSERT_EQ(st[0], "!");
}

TEST(TokenParser, NoCharMixDigits)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse(" 1a a1a a1 1a1 2z");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 0ul);
  ASSERT_EQ(st.size(), 5ul);
}

TEST(TokenParser, BothSidesSpaces)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("      Both  sides  are  spaces      ");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 0ul);
  ASSERT_EQ(st.size(), 4ul);
}

TEST(TokenParser, UTF8)
{
  TokenParser parser;
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("Эта строка в кодировке utf-8.");
  std::vector <uint64_t> dt = parser.getDigitTokens();
  std::vector <std::string> st = parser.getStringTokens();
  ASSERT_EQ(dt.size(), 0ul);
  ASSERT_EQ(st.size(), 5ul);
  ASSERT_EQ(st[0], "Эта");
  ASSERT_EQ(st[1], "строка");
  ASSERT_EQ(st[2], "в");
  ASSERT_EQ(st[3], "кодировке");
  ASSERT_EQ(st[4], "utf-8.");
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}                              

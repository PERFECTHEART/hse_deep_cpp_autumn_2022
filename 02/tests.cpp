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


std::vector <std::string> StringTokens;
std::vector <uint64_t> DigitTokens;

void setStartState(parseState *state) { 
  *state = start;
  StringTokens.clear();
  DigitTokens.clear();
}

void setEndState(parseState *state) {
  *state = end;
}

void runString(std::string str) {
  StringTokens.push_back(str);
}

void runDigit(uint64_t num) {
  DigitTokens.push_back(num);
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
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.Parse("0 18446744073709551615 9999999999999999999");

  ASSERT_EQ(DigitTokens.size(), 3ul);
  ASSERT_EQ(DigitTokens[0], 0ul);
  ASSERT_EQ(DigitTokens[1], 18446744073709551615ul);
  ASSERT_EQ(DigitTokens[2], 9999999999999999999ul);
}

TEST(TokenParser, InvalidNumber)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("18446744073709551616");
  ASSERT_EQ(DigitTokens.size(), 0ul);
  ASSERT_EQ(StringTokens[0], "18446744073709551616");
}

TEST(TokenParser, LeadingZeros)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("00 001 0002 00003 00004 0000000000000000000000000000990");
  ASSERT_EQ(DigitTokens.size(), 6ul);
  ASSERT_EQ(StringTokens.size(), 0ul);
  ASSERT_EQ(DigitTokens[0], 0ul);
  ASSERT_EQ(DigitTokens[1], 1ul);
  ASSERT_EQ(DigitTokens[2], 2ul);
  ASSERT_EQ(DigitTokens[3], 3ul);
  ASSERT_EQ(DigitTokens[4], 4ul);
  ASSERT_EQ(DigitTokens[5], 990ul);
}

TEST(TokenParser, EmptyLine)
{
  TokenParser parser;

  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("");
  ASSERT_EQ(DigitTokens.size() + StringTokens.size(), 0ul);
}

TEST(TokenParser, LineOfSpaces)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse(" \t \n \v \f \r");
  ASSERT_EQ(DigitTokens.size() + StringTokens.size(), 0ul);
}

TEST(TokenParser, SingleSpace)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse(" ");
  ASSERT_EQ(DigitTokens.size(), 0ul);
  ASSERT_EQ(StringTokens.size(), 0ul);
}

TEST(TokenParser, SingleDigit)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("7");
  ASSERT_EQ(DigitTokens.size(), 1ul);
  ASSERT_EQ(DigitTokens[0], 7ul);
  ASSERT_EQ(StringTokens.size(), 0ul);
}

TEST(TokenParser, SingleChar)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("!");
  ASSERT_EQ(DigitTokens.size(), 0ul);
  ASSERT_EQ(StringTokens.size(), 1ul);
  ASSERT_EQ(StringTokens[0], "!");
}

TEST(TokenParser, NoCharMixDigits)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse(" 1a a1a a1 1a1 2z");
  ASSERT_EQ(DigitTokens.size(), 0ul);
  ASSERT_EQ(StringTokens.size(), 5ul);
}

TEST(TokenParser, BothSidesSpaces)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("      Both  sides  are  spaces      ");
  ASSERT_EQ(DigitTokens.size(), 0ul);
  ASSERT_EQ(StringTokens.size(), 4ul);
}

TEST(TokenParser, UTF8)
{
  TokenParser parser;
  parser.SetStartCallback(&setStartState);
  parser.SetDigitTokenCallback(&runDigit);
  parser.SetStringTokenCallback(&runString);
  parser.Parse("Эта строка в кодировке utf-8.");
  ASSERT_EQ(DigitTokens.size(), 0ul);
  ASSERT_EQ(StringTokens.size(), 5ul);
  ASSERT_EQ(StringTokens[0], "Эта");
  ASSERT_EQ(StringTokens[1], "строка");
  ASSERT_EQ(StringTokens[2], "в");
  ASSERT_EQ(StringTokens[3], "кодировке");
  ASSERT_EQ(StringTokens[4], "utf-8.");
}

int main(int argc, char **argv)
{
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}                              

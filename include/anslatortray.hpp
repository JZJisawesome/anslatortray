/* Anslatortray
 * A library to translate to and from pig latin
 *
 * Copyright 2018 John Jekel
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>
*/


#ifndef ANSLATORTRAY_HPP
#define ANSLATORTRAY_HPP

#include <string>
#include <sstream>
#include <cstdint>
#include <iterator>
#include <algorithm>

#include <cstring>

#include <iostream>


namespace anslatortray
{
namespace Characters
{
namespace Letters
{
constexpr char ALL[] {"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ"};

constexpr char VOWELS[] {"aAeEiIoOuU"};
constexpr char Y[] {"yY"};
constexpr char VOWELS_WITH_Y[] {"aAeEiIoOuUyY"};
}

constexpr char PUNCTUAION[] {"?!.,;:()/\"\\"};
}

inline std::string wordToPig(const std::string &englishWord);
//inline constexpr char *wordToPig(char *englishWord);
#if __cplusplus >= 201703L
inline std::string_view wordToPigSV(std::string_view englishWord);
#endif

inline std::string wordToPigPunctuation(const std::string &englishWord);

inline std::string changeWords(const std::string &words, std::string wordChanger (const std::string &word));

inline std::string wordsToPig(const std::string &englishWords);
inline std::string sentenceToPig(const std::string &englishSentence);

inline std::string attemptWordToEnglish(const std::string &pig, std::uint64_t beginningVowels = 1);


}

namespace anslatortray
{
std::string wordToPig(const std::string &englishWord)
{
    std::string::size_type result {englishWord.find_first_of(Characters::Letters::VOWELS_WITH_Y)};

    if (result != std::string::npos)
    {
        if (result == 0)
            return englishWord + "way";
        else
        {
            //word without prefix + word until 1st vowel + "ay"
            std::string finished {englishWord.substr(result)};
            finished += englishWord.substr(0, result);
            finished += "ay";

            std::transform(std::begin(finished), std::end(finished), std::begin(finished), tolower);

            return finished;
        }
    }
    else
        return englishWord;
}

/*
constexpr char *wordToPig(char *englishWord)
{
    auto wordSize {std::strlen(englishWord)};

    auto result {std::find_first_of(englishWord[0], englishWord[wordSize], Characters::Letters::VOWELS_WITH_Y[0], Characters::Letters::VOWELS_WITH_Y[std::strlen(Characters::Letters::VOWELS_WITH_Y)])};

    std::cout << result << std::endl;

    if (result != '\n')
    {
        if (result == 0)
            return std::strcat(englishWord, "way");
        else
        {
            //std::string noPrefix {englishWord.substr(result)};

            //std::string suffix {englishWord.substr(0, result)};

            //suffix += {"ay"};

            //std::string finished {noPrefix + suffix};

            //std::transform(std::begin(finished), std::end(finished), std::begin(finished), tolower);



            return "not done";
        }
    }

    return englishWord;
}
*/

std::string wordToPigPunctuation(const std::string &englishWord)
{
    std::string::size_type wordStartIndex {englishWord.find_first_of(Characters::Letters::ALL)};
    std::string::size_type wordEndIndex {englishWord.find_last_of(Characters::Letters::ALL)};

    //prefix punctuation + pigified word + suffix punctuation
    std::string finished {englishWord.substr(0, wordStartIndex)};
    finished += wordToPig(englishWord.substr(0, wordEndIndex + 1));
    finished += englishWord.substr(wordEndIndex + 1);
    return finished;
}

std::string changeWords(const std::string &words, std::string wordChanger (const std::string &word))
{
    std::stringstream wordStream {words};
    std::string pigWords {""};

    //std::transform(std::istream_iterator<std::string> {wordStream}, {}, std::begin(pigWords), [](std::string word){return wordToPig(word);});

    std::string word {""};

    while (wordStream >> word)
    {
        pigWords += wordChanger(word);
        pigWords += " ";
    }

    /*
    for (std::string &word : wordStream)
    {
        pigWords += wordToPig(word);
    }
    */

    return pigWords;
}

std::string wordsToPig(const std::string &englishWords)
{
    return changeWords(englishWords, wordToPig);
}

std::string sentenceToPig(const std::string &englishSentence)
{
    return changeWords(englishSentence, wordToPigPunctuation);
}

std::string attemptWordToEnglish(const std::string &pig, std::uint64_t beginningVowels)
{
    std::string noAy {pig.substr(0, pig.size() - 2)};

    std::string noPrefix {noAy.substr(0, noAy.size() - beginningVowels)};
    std::string prefix {noAy.substr(noAy.size() - beginningVowels)};

    return prefix + noPrefix;
}

#if __cplusplus >= 201703L
#endif
}

#endif // ANSLATORTRAY_H

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
inline std::string wordToPigPunctuation(const std::string &englishWord);
inline std::string multipleWordsToPig(const std::string &englishWords);
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
            std::string noPrefix {englishWord.substr(result)};

            std::string suffix {};

            for (std::string::size_type i = 0; i != result; i++)
            {
                suffix += englishWord[i];
            }

            suffix += "ay";

            std::string finished {noPrefix + suffix};

            std::transform(std::begin(finished), std::end(finished), std::begin(finished), tolower);

            return finished;
        }
    }
    else
        return englishWord;
}

std::string wordToPigPunctuation(const std::string &englishWord)
{
    std::string::size_type wordStartIndex {englishWord.find_first_of(Characters::Letters::ALL)};

    std::string prefix {englishWord.substr(0, wordStartIndex)};
    std::string rest {englishWord.substr(wordStartIndex)};

    std::string::size_type wordEndIndex {rest.find_last_of(Characters::Letters::ALL)};

    std::string word {rest.substr(0, wordEndIndex + 1)};
    std::string suffix {rest.substr(wordEndIndex + 1)};


    return prefix + wordToPig(word) + suffix;
}

std::string multipleWordsToPig(const std::string &englishWords)
{
    std::stringstream wordStream {englishWords};
    std::string pigWords {""};

    //std::transform(std::istream_iterator<std::string> {wordStream}, {}, std::begin(pigWords), [](std::string word){return wordToPig(word);});

    std::string word {""};

    while (wordStream >> word)
    {
        pigWords += wordToPig(word) + " ";
    }

    /*
    for (std::string &word : wordStream)
    {
        pigWords += wordToPig(word);
    }
    */

    return pigWords;
}

std::string sentenceToPig(const std::string &englishSentence)
{
    std::stringstream wordStream {englishSentence};
    std::string pigWords {""};

    //std::transform(std::istream_iterator<std::string> {wordStream}, {}, std::begin(pigWords), [](std::string word){return wordToPig(word);});

    std::string word {""};

    while (wordStream >> word)
    {
        pigWords += wordToPigPunctuation(word) + " ";
    }

    /*
    for (std::string &word : wordStream)
    {
        pigWords += wordToPig(word);
    }
    */

    return pigWords;
}

std::string attemptWordToEnglish(const std::string &pig, std::uint64_t beginningVowels)
{
    std::string noAy {pig.substr(0, pig.size() - 2)};

    std::string noPrefix {noAy.substr(0, noAy.size() - beginningVowels)};
    std::string prefix {noAy.substr(noAy.size() - beginningVowels)};

    return prefix + noPrefix;
}
}

#endif // ANSLATORTRAY_H

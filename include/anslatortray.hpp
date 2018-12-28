/* Anslatortray
 * A simple, header-only library to translate from English to Pig Latin.
 *
 * MIT License
 *
 * Copyright (c) 2018 John Jekel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:

 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
*/

/** \mainpage
 *
 * \section welcome_sec Anslatortray
 *
 * Welcome to the Anslatortray Documentation!
 *
 * <a href="https://github.com/JZJisawesome/anslatortray">Anslatortray Github Repository</a>.
 *
 * \author John Jekel
 * \date 2018
 * \copyright <a href="https://raw.githubusercontent.com/JZJisawesome/anslatortray/master/LICENSE">MIT Licence</a>
 */
#ifndef ANSLATORTRAY_HPP
#define ANSLATORTRAY_HPP

#include <string>
#include <sstream>
#include <cstdint>
#include <iterator>
#include <algorithm>
#include <cctype>

#include <cstring>

/** \namespace anslatortray
 *
 * \brief Namespace containing functions to translate from English to Pig Latin.
*/
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
}

inline std::string wordToPig(const std::string &englishWord);
//inline constexpr char *wordToPig(char *englishWord);
#if __cplusplus >= 201703L
inline std::string_view wordToPigSV(std::string_view englishWord);
#endif

inline std::string smartWordToPig(const std::string &englishWord);

inline std::string changeWords(const std::string &words, std::string wordChanger (const std::string &word));

inline std::string wordsToPig(const std::string &englishWords);
inline std::string translate(const std::string &englishSentence);

inline std::string attemptWordToEnglish(const std::string &pig, std::uint64_t beginningVowels = 1);


}

namespace anslatortray
{
std::string wordToPig(const std::string &englishWord)
{
    std::string::size_type result {englishWord.find_first_of(Characters::Letters::VOWELS_WITH_Y)};//fixme depends on word

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

std::string smartWordToPig(const std::string &englishWord)
{
    std::string::size_type wordStartIndex {englishWord.find_first_of(Characters::Letters::ALL)};

    std::string::size_type wordEndIndex {englishWord.find('\'')};
    if (wordEndIndex == std::string::npos)
        wordEndIndex = {englishWord.find_last_of(Characters::Letters::ALL) + 1};


    std::string pig {wordToPig(englishWord.substr(wordStartIndex, wordEndIndex - wordStartIndex))};//2nd param is count between start and end
    std::transform(std::begin(pig), std::end(pig), std::begin(pig), tolower);//make all letters in new word lower for now//fixme why no std::tolower
    if (std::isupper(englishWord.substr(wordStartIndex, wordEndIndex - wordStartIndex)[0]))//if original word had capital
        pig[0] = {static_cast<char> (std::toupper(pig[0]))};//new word should have capital


    //prefix punctuation + pigified word + suffix punctuation
    std::string finished {englishWord.substr(0, wordStartIndex)};
    finished += pig;
    finished += englishWord.substr(wordEndIndex);

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

std::string translate(const std::string &englishSentence)
{
    return changeWords(englishSentence, smartWordToPig);
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

/** \mainpage
 *
** \section welcome_sec Anslatortray 0.3.3
 *
 * Welcome to the Anslatortray Documentation!
 *
 * A simple, header-only library to translate from English to Pig Latin.
 *
 * <a href="https://github.com/JZJisawesome/anslatortray">Anslatortray Github Repository</a>.
 *
 * <a href="https://en.wikipedia.org/wiki/Pig_Latin">Wikipedia's definition</a> of Pig Latin is "a language game in which words in English are altered, usually by adding a fabricated suffix or by moving the onset or initial consonant or consonant cluster of a word to the end of the word and adding a vocalic syllable to create such a suffix."
 *
 * Essentially, the word is reorganized in an effort to hide its true meaning, which can be lots of fun!
 * The Anslatortray library can help out by converting any English text into Pig Latin quickly and easily.
 *
 * Try compiling this example code (with C++11):
 *
 * \code
 * #include "anslatortray.hpp"
 * #include <iostream>
 *
 * void main()
 * {
 *      std::cout << anslatortray::translate("Hello World!") << std::endl;
 *      return 0;
 * }
 * \endcode
 *
 *
** <h3> Anslatortray Code and Documentation Licence </h3>
 *
 * MIT License
 *
 * Copyright (c) 2018-2019 John Jekel
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
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
 *
 * \author John Jekel
 * \date 2018-2019
 * \copyright <a href="https://raw.githubusercontent.com/JZJisawesome/anslatortray/master/LICENSE">MIT Licence</a>
 */
#ifndef ANSLATORTRAY_HPP
#define ANSLATORTRAY_HPP

#if __cplusplus >= 201103L//supports C++11 and later for now


/* Headers */
#include <string>
#include <cstdint>
#include <algorithm>
#include <cctype>
//#include <cstring>
//#include <iterator>


#define ANSLATORTRAY_VERSION 0.3.3

/** \namespace anslatortray
 *
 * \brief Namespace containing functions to translate from English to Pig Latin.
*/
namespace anslatortray//Definitions
{
    /* Definitions */

    /* Behavior altering definitions (safe to change) */

    //decent speed boost sacrifising replication of capital letters in translated word in smartWordToPig; recomended based on needs
    //#define ANSLATORTRAY_IGNORE_CAPS

    //extremly small speed boost sacrifising sanity checking; words without vowels/letters will cause problems; not recomended unless speed despratly needed needed
    //#define ANSLATORTRAY_SKIP_SANITY_CHECKS

    //disables uncommonly used functions and other unnecerary things to reduce memory/binary footprints; does little that is actually worth it
    //#define ANSLATORTRAY_TINY_TRANSLATOR

    /** \brief Ending to use if word to translate starts with a vowel */
    constexpr char VOWEL_START_STYLE[] = {"way"};//sometimes "yay" is used


    /* External use definitions */
    /** \brief Translates a single complex English word to Pig Latin. (more robust)
     *
     * Unlike wordToPig, this function also handles punctuation (not seperated by whitespace), singular possesion ('s) and creates similar capatilization to the original english word (unless disabled by defining ANSLATORTRAY_IGNORE_CAPS).\n
     * Imperfect results with plural words (ending in "s") and words with mutiple possesion (ending in "s'")
     *
     * \param englishWord An English word to translate
     * \return The word in Pig Latin
     */
    inline std::string smartWordToPig(const std::string &englishWord);

    /** \brief Uses smartWordToPig and changeWords to perform translation from English to Pig Latin on every word it is given.
     *
     * Replaces all whitespace with a regular space.
     *
     * \param englishText The original English text
     * \return The text translated to Pig Latin
    */
    inline std::string translate(const std::string &englishText);

    /** \brief Translates a single English word to Pig Latin.
     *
     * Moves all constanants until the first vowel to the end of the word and adds "ay".
     * Alternativly, if the word starts with a vowel, "way" is added to the end.
     *
     * \param englishWord An English word to translate
     * \return The word in Pig Latin
     */
    inline std::string wordToPig(const std::string &englishWord);

    #ifndef ANSLATORTRAY_TINY_TRANSLATOR
    /** \brief Uses wordToPig and changeWords to perform dumb translation from English to Pig Latin on every word it is given.
     *
     * Replaces all whitespace with a regular space.\n
     * Disabled by defining ANSLATORTRAY_TINY_TRANSLATOR
     *
     * \param englishWords The original English text
     * \return The text "translated" to Pig Latin (no punctuation, uppercase, or possesion support)
    */
    inline std::string wordsToPig(const std::string &englishWords);
    #endif

    #ifndef ANSLATORTRAY_TINY_TRANSLATOR
    /** \brief Tries to translate a word in Pig Latin back to English.
     *
     * This is hard to do because diffrent English words can be the same in Pig Latin.\n
     * Disabled by defining ANSLATORTRAY_TINY_TRANSLATOR
     *
     * \param pig Word in Pig Latin
     * \param numBeginningConosoants The number of conosonants before the first vowel in the original word. Default 1 (most common)
     * \return Word in English
     */
    inline std::string attemptWordToEnglish(const std::string &pig, std::uint64_t numBeginningConosoants = 1);
    #endif


    /* Internal use definitions */
    /** \brief Arrays of diffrent characters (internal use) */
    namespace Characters
    {
        /** \brief Arrays of diffrent letters (internal use) */
        namespace Letters
        {
            /** \brief Array containing all upper and lower case letters (internal use) */
            constexpr char ALL[] {"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ"};

            /** \brief Array containing all upper and lower case vowels (except for y) (internal use) */
            constexpr char VOWELS[] {"aAeEiIoOuU"};

            #ifndef ANSLATORTRAY_TINY_TRANSLATOR
            /** \brief Array containing all upper and lower case vowels (including y) (internal use) */
            constexpr char VOWELS_WITH_Y[] {"aAeEiIoOuUyY"};
            /** \brief Array containing upper and lower case y (internal use) */
            constexpr char Y[] {"yY"};
            #endif
        }

        /** \brief Array containing diffrent apostrophes (internal use) */
        constexpr char APOSTROPHE[] {"\'"};//should also have ʼ and ’ but unicode does not play nice with std::string::find_last_of
        /** \brief Array containing diffrent types of whitespace (internal use) */
        constexpr char WHITESPACE[] {" \t\v\n\r\f"};
    }

    /** \brief Helper function to perform an operation on all whitespace-seperated words (tokens) given to it.
     *
     * See Characters::WHITESPACE for the types
     *
     * \param words Words (tokens) seperated by whitespace
     * \param wordChanger Function taking a const std::string & as a single parameter and returning a std::string
     * \return Words fed through wordChanger with spaces between them
     */
    inline std::string changeWords(const std::string &words, std::string wordChanger (const std::string &word));
}


/* Implementations */
namespace anslatortray
{
    inline std::string smartWordToPig(const std::string &englishWord)
    {
        //find the start of the actual word in the string
        std::string::size_type wordStartIndex {englishWord.find_first_of(Characters::Letters::ALL)};//after any beginning punctuation

        std::string::size_type wordEndIndex {englishWord.find_last_of(Characters::APOSTROPHE)};//try to find an ending apostrophe for possesion or a contraction, seperate from translation
        if (wordEndIndex == std::string::npos)//if there is no apostrophe
            wordEndIndex = {englishWord.find_last_of(Characters::Letters::ALL) + 1};//find the last letter in the string to use as ending


        //sanity check
        #ifndef ANSLATORTRAY_SKIP_SANITY_CHECKS
        if (wordStartIndex == std::string::npos || wordEndIndex == std::string::npos)//make sure word indexes are sane
            return englishWord;//if not, return original word as there are no letters
        #endif


        //extract word and translate
        std::string actualWord {englishWord.substr(wordStartIndex, wordEndIndex - wordStartIndex)};//2nd param is count between start and end of actual word
        std::string pig {wordToPig(actualWord)};//translate English word


        //capatilization handeling
        #ifndef ANSLATORTRAY_IGNORE_CAPS
        if (std::all_of(std::begin(actualWord), std::end(actualWord), isupper) && actualWord != "I")//if entire original word was uppercase (except for the word "I")//fixme why no std::toupper
            std::transform(std::begin(pig), std::end(pig), std::begin(pig), toupper);//make entire translated word uppercase
        else
        {
            std::transform(std::begin(pig), std::end(pig), std::begin(pig), tolower);//make entire translated word lowercase//fixme why no std::tolower

            if (std::isupper(actualWord[0]))//if original word had an uppercase first letter
                pig[0] = {static_cast<char> (std::toupper(pig[0]))};//new word should have uppercase first letter; have to cast int to char
        }
        #endif

        //prefix punctuation + pigified word + suffix punctuation
        std::string result {englishWord.substr(0, wordStartIndex)};//up to the start of the word
        result += {pig};//translated word from earlier
        result += {englishWord.substr(wordEndIndex)};//from end of the word to the end of the string
        return result;
    }

    inline std::string translate(const std::string &englishText)
    {
        return changeWords(englishText, smartWordToPig);
    }

    inline std::string wordToPig(const std::string &englishWord)
    {
        const std::string::size_type firstVowel {englishWord.find_first_of(Characters::Letters::VOWELS)};//fixme y being a vowel depends on word

        #ifndef ANSLATORTRAY_SKIP_SANITY_CHECKS
        if (firstVowel == std::string::npos)//basic sanity checking
            return englishWord;
        #endif


        if (firstVowel == 0)//word starts with vowel
            return englishWord + VOWEL_START_STYLE;//just add "way" (or something else)//note, += has little to no benifit here
        else
        {
            //word without beginning consononts + beginning consononts + "ay"
            std::string result {englishWord.substr(firstVowel)};
            result += {englishWord.substr(0, firstVowel)};
            result += {"ay"};

            return result;
        }
    }

    #ifndef ANSLATORTRAY_TINY_TRANSLATOR
    inline std::string wordsToPig(const std::string &englishWords)
    {
        return changeWords(englishWords, wordToPig);
    }
    #endif

    #ifndef ANSLATORTRAY_TINY_TRANSLATOR
    inline std::string attemptWordToEnglish(const std::string &pig, std::uint64_t numBeginningConosoants)
    {
        std::string noAy {pig.substr(0, pig.size() - 2)};//try to take off ay (2 characters)

        std::string withoutBeginningConosoants {noAy.substr(0, noAy.size() - numBeginningConosoants)};//take rest of word from front
        std::string beginningConosoants {noAy.substr(noAy.size() - numBeginningConosoants)};//take beginning conosoants from the end

        return beginningConosoants + withoutBeginningConosoants;//put back in proper order
    }
    #endif

    inline std::string changeWords(const std::string &words, std::string wordChanger (const std::string &word))
    {
        std::string newWords {};
        std::string word {};

        std::string::size_type tokenStartIndex {words.find_first_not_of(Characters::WHITESPACE)};//start of token
        std::string::size_type tokenEndIndex {words.find_first_of(Characters::WHITESPACE, tokenStartIndex)};//first whitespace after token

        while (tokenStartIndex != std::string::npos)//no more things to tokenize
        {
            //tokenize
            if (tokenEndIndex == std::string::npos)//if there is no more white space (last token in string)
                word = {words.substr(tokenStartIndex)};//tokenize from last whitespace to end of string
            else//tokenize between start of token the and next found whitespace (2nd param is count between the two)
                word = {words.substr(tokenStartIndex, tokenEndIndex - tokenStartIndex)};


            //preform wordChanger on each word and add space in between
            newWords += {wordChanger(word)};
            newWords += {" "};


            //find placement of next token
            tokenStartIndex = {words.find_first_not_of(Characters::WHITESPACE, tokenEndIndex)};//find the next start of a token after whitespace
            tokenEndIndex = {words.find_first_of(Characters::WHITESPACE, tokenStartIndex)};//fin the next whitespace after start of token
        }

        return newWords;

        //probably best way of doing it (if it worked)
        //std::transform(std::istream_iterator<std::string> {wordStream}, {}, std::begin(pigWords), [](std::string word){return wordToPig(word);});
    }
}

#else
#error Sorry, but for the moment, Anslatortray only supports for C++11 and later. For now, please change your compiliation flags accordinaly
#endif

#endif //ANSLATORTRAY_HPP

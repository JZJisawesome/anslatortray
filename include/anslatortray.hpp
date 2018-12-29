/* Anslatortray 0.3.1
 *
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

#define ANSLATORTRAY_VERSION 0.3.1


#if __cplusplus >= 201103L//supports C++11 and later for now

#include <string>
#include <cstdint>
#include <algorithm>
#include <cctype>
//#include <iterator>
//#include <cstring>
//#include <sstream>


/** \namespace anslatortray
 *
 * \brief Namespace containing functions to translate from English to Pig Latin.
*/
namespace anslatortray//Definitions
{
    /** \brief Translates a single complex English word to Pig Latin. (more robust)
     *
     * Unlike wordToPig, this function also handles punctuation (not seperated by whitespace), singular possesion ('s) and capatilizes the first letter if the original english word was capatilized.\n
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

    /** \brief Uses wordToPig and changeWords to perform dumb translation from English to Pig Latin on every word it is given.
     *
     * Replaces all whitespace with a regular space.
     *
     * \param englishText The original English text
     * \return The text "translated" to Pig Latin (no punctuation, uppercase, or possesion support)
    */
    inline std::string wordsToPig(const std::string &englishWords);

    /** \brief Tries to translate a word in Pig Latin back to english.
     *
     * This is hard to do because diffrent English words can be the same in Pig Latin.
     *
     * \param pig Word in Pig Latin
     * \param numBeginningConosoants The number of conosonants before the first vowel in the original word. Default 1 (most common)
     * \return Word in English
     */
    inline std::string attemptWordToEnglish(const std::string &pig, std::uint64_t numBeginningConosoants = 1);

    /**< Ending to use if word to translate starts with a vowel */
    constexpr char VOWEL_START_STYLE[] = {"way"};//sometimes "yay" is used


    //Features in progress
    #if __cplusplus >= 201402L
        //inline constexpr char *wordToPig(char *englishWord);//futile attempt at compile time translation
    #endif

    #if __cplusplus >= 201703L
        //inline std::string_view wordToPigSV(std::string_view englishWord);
    #endif


    //Internal use
    /**< Arrays of diffrent characters (internal use) */
    namespace Characters
    {
        /**< Arrays of diffrent letters (internal use) */
        namespace Letters
        {
            /**< Array containing all upper and lower case letters (internal use) */
            constexpr char ALL[] {"aAbBcCdDeEfFgGhHiIjJkKlLmMnNoOpPqQrRsStTuUvVwWxXyYzZ"};

            /**< Array containing all upper and lower case vowels (except for y) (internal use) */
            constexpr char VOWELS[] {"aAeEiIoOuU"};
            /**< Array containing all upper and lower case vowels (including y) (internal use) */
            constexpr char VOWELS_WITH_Y[] {"aAeEiIoOuUyY"};
            /**< Array containing upper and lower case y (internal use) */
            constexpr char Y[] {"yY"};
        }

        /**< Array containing diffrent apostrophes (internal use) */
        constexpr char APOSTROPHE[] {"\'"};//should also have ʼ and ’ but unicode does not play nice with std::string::find_last_of
        /**< Array containing diffrent types of whitespace (internal use) */
        constexpr char WHITESPACE[] {" \t\v\n\r\f"};
    }

    /** \brief Helper function to perform an operation on all whitespace-seperated strings given to it.
     *
     * \param words Words (tokens) seperated by whitespace
     * \param wordChanger Function taking a const std::string & as a single parameter and returning a std::string
     * \return Words fed through wordChanger with spaces between them
     */
    inline std::string changeWords(const std::string &words, std::string wordChanger (const std::string &word));
}

//Implementations
namespace anslatortray
{
    std::string smartWordToPig(const std::string &englishWord)
    {
        //find the actual word in the entire string
        std::string::size_type wordStartIndex {englishWord.find_first_of(Characters::Letters::ALL)};//after any beginning punctuation

        std::string::size_type wordEndIndex {englishWord.find_last_of(Characters::APOSTROPHE)};//try to find an ending apostrophe for possesion or a contraction, seperate from translation
        if (wordEndIndex == std::string::npos)//if there is no apostrophe
            wordEndIndex = {englishWord.find_last_of(Characters::Letters::ALL) + 1};//find the last letter in the string to use as ending


        //extract it and translate, sanity checking in the process
        std::string actualWord {};
        if (wordStartIndex != std::string::npos && wordEndIndex != std::string::npos)//make sure word is sane
             actualWord = {englishWord.substr(wordStartIndex, wordEndIndex - wordStartIndex)};//2nd param is count between start and end of actual word
        else
            return englishWord;//this sanity checking takes also care of the other englishWord.substr in the final assembly phase

        std::string pig {wordToPig(actualWord)};//translate English word


        //capatilization handeling
        if (std::all_of(std::begin(actualWord), std::end(actualWord), isupper) && actualWord != "I")//if entire original word was uppercase (except for the word "I")//fixme why no std::toupper
            std::transform(std::begin(pig), std::end(pig), std::begin(pig), toupper);//make entire translated word uppercase
        else
        {
            std::transform(std::begin(pig), std::end(pig), std::begin(pig), tolower);//make entire translated word lowercase//fixme why no std::tolower

            if (std::isupper(actualWord[0]))//if original word had an uppercase first letter
                pig[0] = {static_cast<char> (std::toupper(pig[0]))};//new word should have uppercase first letter; have to cast int to char
        }


        //prefix punctuation + pigified word + suffix punctuation
        std::string result {englishWord.substr(0, wordStartIndex)};//up to the start of the word
        result += {pig};//translated word from earlier
        result += {englishWord.substr(wordEndIndex)};//from end of the word to the end of the string
        return result;
    }

    std::string translate(const std::string &englishText)
    {
        return changeWords(englishText, smartWordToPig);
    }

    std::string wordToPig(const std::string &englishWord)
    {
        const std::string::size_type firstVowel {englishWord.find_first_of(Characters::Letters::VOWELS)};//fixme y being a vowel depends on word

        if (firstVowel != std::string::npos)//basic sanity checking
        {
            if (firstVowel == 0)//word starts with vowel
                return englishWord + VOWEL_START_STYLE;
            else
            {
                //word without beginning consononts + beginning consononts + "ay"
                std::string result {englishWord.substr(firstVowel)};
                result += englishWord.substr(0, firstVowel);
                result += "ay";

                return result;
            }
        }

        return englishWord;
    }

    std::string wordsToPig(const std::string &englishWords)
    {
        return changeWords(englishWords, wordToPig);
    }

    std::string attemptWordToEnglish(const std::string &pig, std::uint64_t numBeginningConosoants)
    {
        std::string noAy {pig.substr(0, pig.size() - 2)};//try to take off ay

        std::string withoutBeginningConosoants {noAy.substr(0, noAy.size() - numBeginningConosoants)};
        std::string beginningConosoants {noAy.substr(noAy.size() - numBeginningConosoants)};

        return beginningConosoants + withoutBeginningConosoants;
    }

#if __cplusplus >= 201402L
    /*
    //futile attempt at compile time translation
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

                return englishWord;
            }
        }


        return englishWord;
    }
    */
#endif

    std::string changeWords(const std::string &words, std::string wordChanger (const std::string &word))
    {
        std::string newWords {""};
        std::string word {""};

        std::string::size_type tokenStartIndex {words.find_first_not_of(Characters::WHITESPACE)};
        std::string::size_type tokenEndIndex {words.find_first_of(Characters::WHITESPACE, tokenStartIndex)};

        while (tokenStartIndex != std::string::npos)//no more things to tokenize
        {
            //tokenize
            if (tokenEndIndex == std::string::npos)//if there is no more white space (last token in string)
                word = {words.substr(tokenStartIndex)};//tokenize from last whitespace to end of string
            else
                word = {words.substr(tokenStartIndex, tokenEndIndex)};//tokenize between last and next whitespace


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

        /*old way, still works but stringstreams are much slower than raw find methods
        std::stringstream wordStream {words};

        while (wordStream >> word)//tokenize words
        {
            //preform wordChanger on each word and add space in between
            newWords += {wordChanger(word)};
            newWords += {" "};
        }
        */
    }
}

#else
#error Sorry, but for the moment, Anslatortray only supports for C++11 and later. For now, please change your compiliation flags accordinaly
#endif

#endif //ANSLATORTRAY_HPP

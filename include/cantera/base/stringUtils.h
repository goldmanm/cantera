/**
 *  @file stringUtils.h Contains declarations for string manipulation
 *       functions within Cantera.
 */
// Copyright 2001  California Institute of Technology

#ifndef CT_STRINGUTILS_H
#define CT_STRINGUTILS_H

#include "ct_defs.h"
#include "cantera/ext/format.h"

#include <string>

namespace Cantera
{

//! Convert a double into a c++ string
/*!
 * @param x double to be converted
 * @param fmt   Format to be used (printf style)
 * @deprecated Unused. To be removed after Cantera 2.3. Use fmt::format instead
 */
std::string fp2str(const double x, const std::string& fmt="%g");

//! Convert an int to a string using a format converter
/*!
 * @param n    int to be converted
 * @param fmt  format converter for an int int the printf command
 * @deprecated Unused. To be removed after Cantera 2.3. Use fmt::format instead
 */
std::string int2str(const int n, const std::string& fmt="%d");

//! Convert an unsigned integer to a string
/*!
 * @param n  int to be converted
 * @deprecated Unused. To be removed after Cantera 2.3. Use fmt::format instead
 */
std::string int2str(const size_t n);

//! Convert a vector to a string (separated by commas)
/*!
 * @param v     vector to be converted
 * @param fmt   Format to be used (printf style) for each element
 * @param sep   Separator
 */
std::string vec2str(const vector_fp& v, const std::string& fmt="%g",
                    const std::string& sep=", ");

//! Strip the leading and trailing white space from a string
/*!
 * The command isprint() is used to determine printable characters.
 *
 * @param   s       Input string
 * @returns a copy of the string, stripped of leading and trailing white space
 */
std::string stripws(const std::string& s);

//! Strip non-printing characters wherever they are
/*!
 * @param s        Input string
 * @returns a copy of the string, stripped of all non- printing characters.
 */
std::string stripnonprint(const std::string& s);

//! Cast a copy of a string to lower case
/*!
 * @param s        Input string
 * @returns a copy of the string, with all characters lowercase.
 */
std::string lowercase(const std::string& s);

//! Parse a composition string into a map consisting of individual
//! key:composition pairs.
/*!
 * Elements present in *names* but not in the composition string will have
 * a value of 0. Elements present in the composition string but not in *names*
 * will generate an exception. The composition is a double. Example:
 *
 * Input is
 *
 *    "ice:1   snow:2"
 *    names = ["fire", "ice", "snow"]
 *
 * Output is
 *             x["fire"] = 0
 *             x["ice"]  = 1
 *             x["snow"] = 2
 *
 * @param ss    original string consisting of multiple key:composition
 *               pairs on multiple lines
 * @param names (optional) valid names for elements in the composition map. If
 *      empty or unspecified, all values are allowed.
 * @return     map of names to values
 */
compositionMap parseCompString(const std::string& ss,
        const std::vector<std::string>& names=std::vector<std::string>());

//! Translate a string into one integer value
/*!
 * No error checking is done on the conversion. The c stdlib function atoi() is
 * used.
 *
 * @param val   String value of the integer
 * @returns     an integer
 */
int intValue(const std::string& val);

//! Translate a string into one doublereal value
/*!
 * No error checking is done on the conversion.
 *
 * @param val   String value of the double
 * @returns     a double
 */
doublereal fpValue(const std::string& val);

//! Translate a string into one doublereal value, with error checking
/*!
 * fpValueCheck is a wrapper around the C++ stringstream double parser. It
 * does quite a bit more error checking than atof() or strtod(), and is quite
 * a bit more restrictive.
 *
 * First it interprets both E, e, d, and D as exponents. stringstreams only
 * interpret e or E as an exponent character.
 *
 * It only accepts a string as well formed if it consists as a single token.
 * Multiple words will raise an exception. It will raise a CanteraError for
 * NAN and inf entries as well, in contrast to atof() or strtod(). The user
 * needs to know that a serious numerical issue has occurred.
 *
 * It does not accept hexadecimal numbers.
 *
 * It always use the C locale, regardless of any locale settings.
 *
 * @param val   String representation of the number
 * @returns     a double
 */
doublereal fpValueCheck(const std::string& val);

//! Parse a name string, separating out the phase name from the species name
/*!
 * Name strings must not contain these internal characters "; \n \t ," Only one
 * colon is allowed, the one separating the phase name from the species name.
 * Therefore, names may not include a colon.
 *
 * @param[in] nameStr    Name string containing the phase name and the species
 *                       name separated by a colon. The phase name is optional.
 *                       example: "silane:SiH4"
 * @param[out] phaseName Name of the phase, if specified. If not specified, a
 *                       blank string is returned.
 * @returns species name. If nameStr is blank an empty string is returned.
 */
std::string parseSpeciesName(const std::string& nameStr, std::string& phaseName);

//! Line wrap a string via a copy operation
/*!
 * @param  s    Input string to be line wrapped
 * @param  len  Length at which to wrap. The default is 70.
 */
std::string wrapString(const std::string& s,
                       const int len=70);

//! Interpret one or two token string as a single double
/*!
 * This is similar to atof(). However, the second token is interpreted as an
 * MKS units string and a conversion factor to MKS is applied.
 *
 * Example: "1.0 atm" results in the number 1.01325e5.
 *
 * @param strSI string to be converted. One or two tokens
 * @returns a converted double
 */
doublereal strSItoDbl(const std::string& strSI);

//! This function separates a string up into tokens according to the location of
//! white space.
/*!
 * White space includes the new line character. tokens are stripped of leading
 * and trailing white space.
 *
 * The separate tokens are returned in a string vector, v.
 *
 * @param oval   String to be broken up
 * @param v     Output vector of tokens.
 */
void tokenizeString(const std::string& oval,
                    std::vector<std::string>& v);

//! Copy the contents of a std::string into a char array of a given length
void copyString(const std::string& source, char* dest, size_t length);

}

#endif

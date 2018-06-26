/*
 *  Copyright 2018 Yihenew Beyene
 *  
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *  
 *  http://www.apache.org/licenses/LICENSE-2.0
 *  
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *  
 */


/*!
 *  \brief Simple configuration parser
 * 
 */


#pragma once

#include <fstream>
#include <string>
#include <vector>

#ifdef USE_ORDERED_MAP
#include <map>
#else
#include <unordered_map>
#endif


namespace ylibs
{
    namespace yconfparser
    {
    
       /*!
        *  \brief Configuration value struct.
        *  
        *  It stores the value and type of a configuration entry.
        *  By default the raw value string is stored.
        *  Additionally, the string is parsed into other types based
        *  on the following rules:
        *
        *  - If the value string is enclosed in square brackets, then
        *    it is treated as an array of comma-separated values where
        *    each item is parsed individually as follows.
        *  - If the value string is enclosed in double-quotes, then
        *    it is assumed that the parameter value is a string. Hence,
        *    no casting operation is needed. However, the leading and
        *    trailing double quotes are removed.
        *  - Or if the value string contains either "TRUE" or "FALSE", then
        *    it is interpreated as a boolean type.
        *  - Or if the value string does not meet any of the above conditions,
        *    then it is assumed to contain either an integer or a floating-
        *    point number. The parser tries to cast the value string as
        *    a floating-point number if it contains a decimal point, and as
        *    an integer otherwise.
        *
        *  - If the parser fails to cast the value string to one of the above
        *    types, then the parser sets the type of the value string as "NO_VAL"
        */
        typedef struct config_struct_
        {
            config_struct_()
            {
                type      = type_::NO_VAL;
                rawString = "";
            }
            
            ~config_struct_()
            {
            }
            
           /*!
            *  \brief  List of supported types
            *
            *   STRING:  Anything enclosed in double-quotes
            *   BOOLEAN: TRUE / FALSE
            *   FLOAT:   A number with decimal point
            *   INTEGER: A number without decimal point
            *
            *   A value can be scalar or array of elements (enclosed in square
            *   brackets, and separated by comma).
            */
            enum class type_{
                NO_VAL,
                STRING,
                BOOLEAN,
                FLOAT,
                INTEGER
            } type;
            
            std::string rawString; // Leading and trailing double quotes (if any) are trimmed.
            
            std::vector<std::string> stringVal;
            std::vector<uint8_t>     boolVal;   // std::vector seams to have specialized implementation for bool type
            std::vector<float>       floatVal;
            std::vector<int>         intVal;
        } config_struct;
        
       /*!
        *  \brief A pair of parameter name and value 
        */
        typedef std::pair<std::string, config_struct> paramValuePair;
        
       /*!
        *  \brief A dictionary of all configuration values indexed by the parameter name
        *         The parameter name is the concatenation of the current and top-level
        *         parameter names separated by a '.'
        *
        *         Example:
        *                     glossary:
        *                        title:  "example glossary"
        *                        GlossDiv: 
        *                            title: "S"
        *
        *   The parameter name for the last entry is "glossary.GlossDiv.title"
        */
        #ifdef USE_ORDERED_MAP
        typedef std::map<std::string, config_struct> configList;        
        #else
        typedef std::unordered_map<std::string, config_struct> configList;
        #endif
        
       /*!
        *  \brief Removes all leading white-spaces until it finds a non-white space
        *         character. TABs are not considered as white spaces
        *
        *  \param  w_string A raw string to be trimmed.
        *  \return Trimmed string
        */
        static std::string trimmLeadingSpaces(std::string w_string)
        {
            if(w_string.empty())
            {
                return "";
            }
            else
            {
                size_t n = w_string.find_first_not_of(' ');
                return n==std::string::npos? "" : w_string.substr(n);
            }
        }
        
       /*!
        *  \brief Removes all leading TABs
        *
        *  \param  w_string A raw string to be trimmed.
        *  \return Trimmed string
        */
        static std::string trimmLeadingTabs(std::string w_string)
        {
            if(w_string.empty())
            {
                return "";
            }
            else
            {
                size_t n = w_string.find_first_not_of('\t');
                return n==std::string::npos? "" : w_string.substr(n);
            }
        }
        
       /*!
        *  \brief Removes all trailing white-spaces until it finds a non-white space
        *         character. TABs are not considered as white spaces
        *
        *  \param  w_string A raw string to be trimmed.
        *  \return Trimmed string
        */
        static std::string trimmTrailingSpaces(std::string w_string)
        {
            if(w_string.empty())
            {
                return "";
            }
            else
            {
                size_t n = w_string.find_last_not_of(' ');
                return n==std::string::npos? "" : w_string.substr(0,n+1);
            }
        }
        
       /*!
        *  \brief  Removes leading and trailing white-spaces.
        *
        *  \param  w_string A raw string to be trimmed.
        *  \return Trimmed string
        */
        static std::string trimmWhiteSpaces(std::string w_string)
        {
            return trimmTrailingSpaces(trimmLeadingSpaces(w_string));
        }
        
       /*!
        *  \brief  Parses a raw string as string type
        *
        *  \param  w_string A raw string to be parsed.
        *  \param  w_val    Parsed string value (if successful)
        *  \return          Flag indicates success or failure
        */
        static bool parseAsString(std::string & w_string, std::string & w_val)
        {
            bool retVal = false;
            
            if(w_string[0] == '\"')
            {
                if(w_string.size() > 2)
                {
                    if(w_string.back() == '\"')
                    {
                        w_val  = w_string.substr(1,w_string.size()-2);
                        retVal = true;
                    }
                }
            }
            
            return retVal;
        }
        
        
       /*!
        *  \brief  Parses a raw string as boolean type
        *
        *  \param  w_string A raw string to be parsed.
        *  \param  w_val    Parsed boolean value (if successful)
        *  \return          Flag indicates success or failure
        */
        static bool parseAsBoolean(std::string & w_string, uint8_t &w_val)
        {
            bool retVal = false;
            
            if(w_string.find("TRUE") != std::string::npos)
            {
                // Boolean TRUE
                w_val  = true;
                retVal = true;
            }
            else if(w_string.find("FALSE") != std::string::npos)
            {
                // Boolean FALSE
                w_val  = false;
                retVal = true;
            }
            
            return retVal;
        }
        
       /*!
        *  \brief  Parses a raw string as floating-point type
        *
        *  \param  w_string A raw string to be parsed.
        *  \param  w_val    Parsed floating-point value (if successful)
        *  \return          Flag indicates success or failure
        */
        static bool parseAsFloat(std::string & w_string, float &w_val)
        {
            bool retVal = false;
        
            if(w_string.find(".") != std::string::npos)
            {
                try
                {
                    w_val  = std::stof (w_string);
                    retVal = true;
                }
                catch(...) {}
            }
            
            return retVal;
        }
        
       /*!
        *  \brief  Parses a raw string as integer type
        *
        *  \param  w_string A raw string to be parsed.
        *  \param  w_val    Parsed integer value (if successful)
        *  \return          Flag indicated success or failure
        */
        static bool parseAsInteger(std::string & w_string, int &w_val)
        {
            bool retVal = false;
        
            if(w_string.find(".") != std::string::npos)
            {
                try
                {
                    w_val  = std::stoi (w_string);
                    retVal = true;
                }
                catch(...) {}
            }
            
            return retVal;
        }  
        
       /*!
        *  \brief  Parses a single line and returns a paramtername-value pair.
        *          Parameters names can be made up of multiple words separated
        *          by white spaces. Indentations can be with white spaces or TABs.
        *          TABS between white spaces (or vice versa) are not allowed.
        *          Every TAB in indentations is counted as one white space.
        *
        *          Every parameter-value pair is separated with a colon.
        *          A field might not be followed by a value after the colon
        *          in which case the type is set to "NO_VAL", and a sub-field is
        *          expected to exist.
        *
        *          Note: Every parameter name & value (including arrays) pair is
        *                expected to span a single line.
        *
        *  \param  w_line A pointer to the single-line string
        *  \param  w_padding Number of indentation white spaces / TABs
        *  \return An std::pair of paramter name & value
        */
        static paramValuePair  parseLine(std::string *w_line, size_t &w_padding)
        {
            // Set default values
            config_struct config;
            w_padding        = 0;
            config.type      = config_struct::type_::NO_VAL;
            config.rawString = "";
            
            paramValuePair pvPair = std::make_pair("", config);
            
            // Check if it is empty
            if(w_line->empty())
            {
                return pvPair;
            }
            
            std::string trimmed_  = trimmLeadingSpaces(*w_line);
            std::string trimmed  = trimmLeadingTabs(trimmed_);
            w_padding = w_line->size() - trimmed.size();
            
            if(trimmed.empty())
            {
                return pvPair;
            }
            
            if(trimmed[0] == ' ')
            {
                fprintf(stderr, "Mixing of TAB(s) and white space(s) is not allowed\n");
                return pvPair;
            }
            
            if(trimmed_.size() != trimmed.size() && trimmed_.size() != w_line->size())
            {
                fprintf(stderr, "Warning: mix of TAB(s) and white space(s)\n");
            }
            
            // Check if it is a comment
            if(trimmed.find('#') == 0)
            {
                return pvPair;
            }
            
            // Every valid parameter-value pair should be separated by a colon (:)
            std::string::size_type n;
            
            n = trimmed.find(':');
            
            if(n == std::string::npos || n == 0)
            {
                fprintf(stderr, "%s\n", ("Invalid    ---" + *w_line).c_str());
                return pvPair;  // Invalid
            }
            
            std::string paramString = trimmWhiteSpaces(trimmed.substr(0,n));
            std::string valString   = n < trimmed.size()? trimmWhiteSpaces(trimmed.substr(n+1)) : "";
            
            if(paramString.empty())
            {
                fprintf(stderr, "Invalid: Missing param");
                return pvPair;
            }
            
            pvPair.first = paramString;
            pvPair.second.rawString = valString;
            
            std::vector<std::string> stringVals(1,"");
            std::vector<uint8_t>     boolVals(1,false);
            std::vector<float>       floatVals(1,0.0);
            std::vector<int>         intVals(1,0);
            
            // Parse value
            if(valString.empty())
            {
                // No val
            }
            else if (parseAsString(valString, stringVals.back()))
            {
                pvPair.second.type      = config_struct::type_::STRING;
                pvPair.second.stringVal = stringVals;
            }
            else if(valString[0] == '[' && valString.back() == ']')
            {
                // Array
                if(valString.size() < 3)
                {
                    return pvPair; // Empty array
                }
                
                std::string elements = valString.substr(1,valString.size()-2);
                config_struct::type_ elementType = config_struct::type_::NO_VAL;
                
                while(true)
                {
                    std::string::size_type n = elements.find(',');
                    std::string element      = n==std::string::npos? elements : elements.substr(0,n);
                    
                    element = trimmWhiteSpaces(element);
                    
                    if (parseAsString(element, stringVals.back()))
                    {
                        elementType = config_struct::type_::STRING;
                        stringVals.push_back("");
                    }
                    else if (parseAsBoolean(element, boolVals.back()))
                    {
                        // Boolean TRUE
                        elementType = config_struct::type_::BOOLEAN;
                        boolVals.push_back(false);
                    }
                    else if (parseAsFloat(element, floatVals.back()))
                    {
                        // Floating-point number
                        elementType = config_struct::type_::FLOAT;
                        floatVals.push_back(0);
                    }
                    else if (parseAsInteger(element, intVals.back()))
                    {
                        // Integer
                        elementType = config_struct::type_::INTEGER;
                        intVals.push_back(0);
                    }
                    else
                    {
                        fprintf(stderr, "Unknown type: %s\n", element.c_str());
                        break;
                    }
                    
                    if(pvPair.second.type == config_struct::type_::NO_VAL)
                    {
                        pvPair.second.type = elementType;
                    }
                    else if(pvPair.second.type != elementType)
                    {
                        fprintf(stderr, "Array entries should have the same type\n");
                        break;
                    }
                    
                    if(n==std::string::npos)
                    {
                        break;
                    }
                    
                    if((n+1) >= elements.size())
                    {
                        break;
                    }
                    
                    elements = trimmWhiteSpaces(elements.substr(n+1));
                }
                
                // Copy parsed array elements
                if(elementType != config_struct::type_::NO_VAL)
                {
                    switch (elementType)
                    {
                        case config_struct::type_::STRING:
                            stringVals.pop_back(); // Remove the last unassigned element
                            pvPair.second.stringVal = stringVals;
                            break;
                        case config_struct::type_::BOOLEAN:
                            boolVals.pop_back(); // Remove the last unassigned element
                            pvPair.second.boolVal = boolVals;
                            break;
                        case config_struct::type_::FLOAT:
                            floatVals.pop_back(); // Remove the last unassigned element
                            pvPair.second.floatVal = floatVals;
                            break;
                        default: // config_struct::type_::INTEGER:
                            intVals.pop_back(); // Remove the last unassigned element
                            pvPair.second.intVal = intVals;
                            break;
                    }
                }
                
            }
            else if (parseAsBoolean(valString, boolVals.back()))
            {
                // Boolean TRUE
                pvPair.second.type    = config_struct::type_::BOOLEAN;
                pvPair.second.boolVal = boolVals;
            }
            else if (parseAsFloat(valString, floatVals.back()))
            {
                // Floating-point number
                pvPair.second.type     = config_struct::type_::FLOAT;
                pvPair.second.floatVal = floatVals;
            }
            else if (parseAsInteger(valString, intVals.back()))
            {
                // Integer
                pvPair.second.type      = config_struct::type_::INTEGER;
                pvPair.second.intVal = intVals;
            }
            
            return pvPair;            
        }
        
       /*!
        *  \brief  Parses a configuration file
        *
        *  The configuration format is hierarchical where every sub-field
        *  is preceeded with indentation white-space. All sub-fields
        *  belonging to the same parent should have equal indentation.
        *  Every parameter-value pair is separated with a colon. A field
        *  might not be followed by a value after the colon, in which case
        *  sub-fields are expected to exist. Moreover, a field might have a
        *  value and one or more sub-fields.
        *
        *  \param  w_fileName Configuration file name
        *  \return A dictionary of paramter names and associated values
        */
        static configList parseFile(std::string w_fileName)
        {
            std::ifstream       file (w_fileName.c_str());
            std::string         line;
            configList          config;
            paramValuePair      pvPair;
            std::string         fullParamName("");
            std::vector<size_t> paramLengths;
            std::vector<size_t> paddingHistory;
            size_t              currentPadding = 0;
            
            // Check if the file is open
            if (!file.is_open())
            {
                fprintf(stderr, "%s\n", ("error while opening file " + w_fileName).c_str());
                return config;
            }

            // Parse the file line-by-line
            while(getline(file, line))
            {
                pvPair = parseLine(&line, currentPadding);
                
                if(pvPair.first.empty() || pvPair.second.type == config_struct::type_::NO_VAL)
                {
                    continue; // Ignore
                }
                
                if(!paddingHistory.empty())
                {
                    if(currentPadding <= paddingHistory.back()) // not sub-field
                    { 
                        // Rewind
                        while(paddingHistory.size() > 0)
                        {
                            paddingHistory.pop_back();
                            fullParamName = fullParamName.substr(0, fullParamName.size() - paramLengths.back());
                            paramLengths.pop_back();
                            
                            if(currentPadding > paddingHistory.back())
                            {
                                break;
                            }
                        }
                    }
                }
                    
                paddingHistory.push_back(currentPadding);
                paramLengths.push_back(1 + pvPair.first.size());
                fullParamName += fullParamName.empty()? pvPair.first : "." + pvPair.first;
                
                // Check if the current parameter has an associated value
                if(pvPair.second.type != config_struct::type_::NO_VAL)
                {
                    config.insert({fullParamName, pvPair.second});
                }
            }
            
            // Check whether error has occured while reading the file
            if (file.bad())
            {
                fprintf(stderr, "%s\n", ("Error while reading file " + w_fileName).c_str());
            }
            
            file.close();
            return config;
        }

        static void printConfiguration(configList w_configList)
        {

            for(auto& c: w_configList)
            {
                std::cout << c.first;
                switch(c.second.type)
                {
                    case ylibs::yconfparser::config_struct::type_::BOOLEAN:

                        std::cout << (c.second.boolVal.size() > 1? " = <B>[" : " = <B>");
                        
                        for (unsigned n=0; n<c.second.boolVal.size(); n++)
                        {
                            std::cout << (n>0? ", " : "");
                            std::cout << (c.second.boolVal[n]? "TRUE" : "FALSE");
                        }
                        
                        std::cout << (c.second.boolVal.size() > 1? "]" : "") << std::endl;
                        break;
                        
                    case ylibs::yconfparser::config_struct::type_::STRING:
                    
                        std::cout << (c.second.stringVal.size() > 1? " = <S>[" : " = <S>");
                        
                        for (unsigned n=0; n<c.second.stringVal.size(); n++)
                        {
                            std::cout << (n>0? ", \"" : "\"");
                            std::cout << c.second.stringVal[n] <<"\"";
                        }
                        
                        std::cout << (c.second.stringVal.size() > 1? "]" : "") << std::endl;
                        break;
                        
                    case ylibs::yconfparser::config_struct::type_::FLOAT:
                    
                        std::cout << (c.second.floatVal.size() > 1? " = <F>[" : " = <F>");
                        
                        for (unsigned n=0; n<c.second.floatVal.size(); n++)
                        {
                            std::cout << (n>0? ", " : "");
                            std::cout << c.second.floatVal[n];
                        }
                        
                        std::cout << (c.second.floatVal.size() > 1? "]" : "") << std::endl;
                        break;
                        
                    case ylibs::yconfparser::config_struct::type_::INTEGER:
                    
                        std::cout << (c.second.intVal.size() > 1? " = <I>[" : " = <I>");
                        
                        for (unsigned n=0; n<c.second.intVal.size(); n++)
                        {
                            std::cout << (n>0? ", " : "");
                            std::cout << c.second.intVal[n];
                        }
                        
                        std::cout << (c.second.intVal.size() > 1? "]" : "") << std::endl;
                        break;
                }
            }
        }
        
    } // namespace yconfparser
    
} // namespace ylibs



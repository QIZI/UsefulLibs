#ifndef BITBOOL_H
#define BITBOOL_H
/******************************  <Zlib>  **************************************
 * Copyright (c) 2016 Martin Baláž 
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

/******************************************************************//**
 *
 * @file   bitbool.h
 * @author Martin Baláž (qizi94@gmail.com)
 * @date   August, 2016
 * @brief  true/false states determined by bit states.
 *
 * Writing and reading true/false states packed in to single or multi
 * byte variables.
 *
 * Example:
 * bool b[8] //sizeof 8 Bytes
 * BOOL8 b   //sizeof 1 byte
 *
 * -same amout of information stored with less memory usage
 * -with optimalization such as -o2 and higher in GCC enabled,
 * there is little to none overhead in comparasion with 8 boolean array.
 *
 * NOTE: avoid usage of "[ ]" or "( )" operator for bit access,
 * can cause insufficient code and overhead.
 * Use b0...b7...b15...b31 members instead.
 *
 ***********************************************************************/
 
#include <inttypes.h>

/**************************************************//**
 * Force function with "inline" keyword to be inlined *
 ******************************************************/

#ifdef __GNUC__  ///gcc compiler check
#define inline inline __attribute__((always_inline))
#endif


/****************************************************//**
 * Argument counter for BOOL[8|16|32] macro overloading *
 ********************************************************/
#define _ARGS(_0, _1, _2, _3, _4, _5, _6, _7, _8, _9,_10,_11,_12,_13,_14,_15,_16,_17,_18,_19,_20,_21,_22,_23,_24,_25,_26,_27,_28,_29,_30,_31,_32,_33, ...) _33
#define __NARGS(...) _ARGS(__VA_ARGS__, 33,32,31,30,29,28,27,26,25,24,23,22,21,20,19,18,17,16,15,14,13,12,11,10,9,8,7,6,5,4,3,2, 1, 0)






#define BOOL8_BIT_SIZE 8
#define BOOL8_BYTE_SIZE (BOOL8_BIT_SIZE / 8)

/*****************************************************//**
 *
 *  desc here
 *  
 *
 *  Usage:
 *
 *  BOOL8 varName(b0,...,b7)
 *
 *  varName.b0=[true/false]
 *
 *  ...
 *
 *  varName.b7=[true/false]
 *
 *  varName( [index] ) - return bit according to index
 *
 *  varname[ [index] ] - equivalent to varName([index])
 *
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL16
 *
 *  varName.size(void) - return constant count of bits
 *
 *  varName = [uint8_t] - overloaded operator will make easier
 *  to copy data from uint8_t
 *
 *
 ***********************************************************/

struct BOOL8{

    union{
        struct{
        //@{
    	/** Anonimous struct that represents b	its numbered from 0 to 7 */
		uint8_t b0 :1;
		uint8_t b1 :1;
		uint8_t b2 :1;
		uint8_t b3 :1;
		uint8_t b4 :1;
		uint8_t b5 :1;
		uint8_t b6 :1;
		uint8_t b7 :1;
		//@}
	};
	uint8_t value;
     };
    BOOL8() = default;
    /*****************************************************//**
    * A constructor
    * sets bits from left to right indead of right to left
    * @param b0 - b7 bits that can be set
    * @note Constructor have default parameters.
    ***********************************************************/
    BOOL8(const bool b0, const bool b1 = 0, const bool b2 = 0, const bool b3 = 0, const bool b4 = 0,
     const bool b5 = 0, const bool b6 = 0, const bool b7 = 0)
     : b0(b0) ,b1(b1), b2(b2), b3(b3), b4(b4), b5(b5), b6(b6), b7(b7){}

    /*****************************************************//**
    * Overloaded operator that can set any bit to 1 or 0.
    * 
    * @param index - index that represents offset from the first bit
    * to the left
    * @param bState - bit state that will be set to the specified bit
    * @note Runtime bit operations.
    ***********************************************************/
    void operator ()(const uint8_t index, const bool bState){
        value = (value & ( ~(0x01 << index))) | (bState << index);
    }
	
    /*****************************************************//**
    * Overloaded operator that will return bit state at the index's offset
    * 
    * @param index - index that represents offset from the first bit
    * to the left
    * @return - state of the specified bit
    * @note Runtime bit operations.
    * @note Same as operator []
    ***********************************************************/
    inline bool operator () (const uint8_t index) const{
        return (*this)[index];
    }
    /*****************************************************//**
    * Overloaded operator that will return bit state at the index's offset.
    * 
    * @param index - index that represents offset from the first bit
    * to the left
    * @return - state of the specified bit
    * @note Runtime bit operations.
    * @note Same as operator ()	.
    ***********************************************************/
    bool operator [] (const uint8_t index) const{
        return ((value >> index) & 0x01);
    }
    
    /*****************************************************//**
    * Overloaded operator that will copy val to value.
    * 
    * @param val - value of variable on left side of = .
    * @return - value of this->value.
    ***********************************************************/
    inline uint8_t operator = (const uint8_t val){return value = val;}
    
    /*****************************************************//**
    * Overloaded operator that will Add value and val.
    * 
    * @param val - value of variable on left side of + 
    * @return - returns the result addition
    ***********************************************************/
    inline uint8_t operator + (const uint8_t val){return (value + val);}
    
    /*****************************************************//**
    * Overloaded operator that will Add value and b8.
    * 
    * @param b8 - value of variable on left side of + 
    * @return - returns the result addition.
    ***********************************************************/
    
    inline uint8_t operator + (const BOOL8 b8){return (value + b8.value);}
    /*****************************************************//**
    * Overloaded operator that will Subtract value and val.
    * 
    * @param val - value of variable on left side of -
    * @return - returns the result subtraction
    ***********************************************************/
    inline uint8_t operator - (const uint8_t val){return (value - val);}
    
    /*****************************************************//**
    * Overloaded operator that will Subtract value and b8.
    * 
    * @param b8 - value of variable on left side of -
    * @return - returns the result subtraction
    ***********************************************************/
    inline uint8_t operator - (const BOOL8 b8){return (value - b8.value);}
    
    /*****************************************************//**
    * Overloaded operator that will Multiply value and val.
    * 
    * @param val - value of variable on left side of *
    * @return - returns the result multipling
    ***********************************************************/
    inline uint8_t operator * (const uint8_t val){return (value * val);}
    
    /*****************************************************//**
    * Overloaded operator that will Multiply value by b8.
    * 
    * @param val - value of variable on left side of *
    * @return - returns the result multipling
    ***********************************************************/
    inline uint8_t operator * (const BOOL8 b8){return (value * b8.value);}
    inline uint8_t operator / (const uint8_t val){return (value / val);}
    inline uint8_t operator / (const BOOL8 b8){return (value / b8.value);}
    
    inline uint8_t operator += (const uint8_t val){return (value += val);}
    inline uint8_t operator += (const BOOL8 b8){return (value += b8.value);}
    inline uint8_t operator -= (const uint8_t val){return (value -= val);}
    inline uint8_t operator -= (const BOOL8 b8){return (value -= b8.value);}
    inline uint8_t operator *= (const uint8_t val){return (value *= val);}
    inline uint8_t operator *= (const BOOL8 b8){return (value *= b8.value);}
    inline uint8_t operator /= (const uint8_t val){return (value /= val);}
    inline uint8_t operator /= (const BOOL8 b8){return (value /= b8.value);}
    
    
    inline uint8_t operator | (const uint8_t val){return (value | val);}
    inline uint8_t operator | (const BOOL8 b8){return (value | b8.value);}
    inline uint8_t operator & (const uint8_t val){return (value & val);}
    inline uint8_t operator & (const BOOL8 b8){return (value & b8.value);}
    inline uint8_t operator ^ (const uint8_t val){return (value ^ val);}
    inline uint8_t operator ^ (const BOOL8 b8){return (value ^ b8.value);}
    inline uint8_t operator << (const uint8_t val){return (value << val);}
    inline uint8_t operator >> (const uint8_t val){return (value >> val);}
    inline uint8_t operator ~ (void){return (~value);}
    
    inline uint8_t operator |= (const uint8_t val){return (value |= val);}
    inline uint8_t operator |= (const BOOL8 b8){return (value |= b8.value);}
    inline uint8_t operator &= (const uint8_t val){return (value &= val);}
    inline uint8_t operator &= (const BOOL8 b8){return (value &= b8.value);}
    inline uint8_t operator ^= (const uint8_t val){return (value ^= val);}
    inline uint8_t operator ^= (const BOOL8 b8){return (value ^= b8.value);}
    
    
    inline const uint8_t size() const{return BOOL8_BIT_SIZE;}

};


/******************************************************************//**
 *
 *  BOOL8_REDEF(...)
 *  Description:
 *  Rename b0...b7 with prefix 'b'
 *
 *  Usage:
 *  BOOL8_REDEF([NAME0],...,[NAME7]) [varName]
 *  varName.bNAME0=[true/false]
 *  ...
 *  varName.bNAME7=[true/false]
 *
 *  varName([index]) - return bit according to index
 *  varname[[index]] - equivalent to varName([index])
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL8
 *  varName.size(void) - return constant count of bits
 *  varName = [uint8_t] - overloaded operator will make easier
 *  to copy data from uint8_t
 *
 *  NOTE: macro is overloaded, you do not need
 *  to rename all 8 bits
 *
 ***********************************************************************/

#define __BOOL8_REDEF1(b0) __BOOL8_REDEF8(b0,1,2,3,4,5,6,7)
#define __BOOL8_REDEF2(b0,b1) __BOOL8_REDEF8(b0,b1,2,3,4,5,6,7)
#define __BOOL8_REDEF3(b0,b1,b2) __BOOL8_REDEF8(b0,b1,b2,3,4,5,6,7)
#define __BOOL8_REDEF4(b0,b1,b2,b3) __BOOL8_REDEF8(b0,b1,b2,b3,4,5,6,7)
#define __BOOL8_REDEF5(b0,b1,b2,b3,b4) __BOOL8_REDEF8(b0,b1,b2,b3,b4,5,6,7)
#define __BOOL8_REDEF6(b0,b1,b2,b3,b4,b5) __BOOL8_REDEF8(b0,b1,b2,b3,b4,b5,6,7)
#define __BOOL8_REDEF7(b0,b1,b2,b3,b4,b5,b6) __BOOL8_REDEF8(b0,b1,b2,b3,b4,b5,b6,7)

#define __BOOL8_REDEF8(b0,b1,b2,b3,b4,b5,b6,b7) \
    struct{\
        union{\
            struct{\
	        		uint8_t b##b0 :1;\
            	uint8_t b##b1 :1;\
            	uint8_t b##b2 :1;\
            	uint8_t b##b3 :1;\
            	uint8_t b##b4 :1;\
            	uint8_t b##b5 :1;\
            	uint8_t b##b6 :1;\
            	uint8_t b##b7 :1;\
             };\
             uint8_t value;\
        };\
            /**Dynamic Write*/\
            void operator ()(const uint8_t index, const bool state){\
                value = (value & ( ~(0x01 << index))) | (state << index);\
            }\
    \
            /**Dynamic Read*/\
            inline bool operator () (const uint8_t index) const{\
                return (*this)[index];\
            }\
            /**Dynamic Read*/\
            bool operator [] (const uint8_t index) const{\
                return ( (value >> index) & 0x01 );\
            }\
    \
            inline uint8_t operator = (const uint8_t val){return value = val;}\
            inline uint8_t operator = (const BOOL8 b8){return value = b8.value;}\
            \
     	    inline uint8_t operator + (const uint8_t val){return (value + val);}\
	    inline uint8_t operator + (const BOOL8 b8){return (value + b8.value);}\
	    inline uint8_t operator - (const uint8_t val){return (value - val);}\
	    inline uint8_t operator - (const BOOL8 b8){return (value - b8.value);}\
	    inline uint8_t operator * (const uint8_t val){return (value * val);}\
	    inline uint8_t operator * (const BOOL8 b8){return (value * b8.value);}\
	    inline uint8_t operator / (const uint8_t val){return (value / val);}\
	    inline uint8_t operator / (const BOOL8 b8){return (value / b8.value);}\
	    \
	    inline uint8_t operator += (const uint8_t val){return (value += val);}\
	    inline uint8_t operator += (const BOOL8 b8){return (value += b8.value);}\
	    inline uint8_t operator -= (const uint8_t val){return (value -= val);}\
	    inline uint8_t operator -= (const BOOL8 b8){return (value -= b8.value);}\
	    inline uint8_t operator *= (const uint8_t val){return (value *= val);}\
	    inline uint8_t operator *= (const BOOL8 b8){return (value *= b8.value);}\
	    inline uint8_t operator /= (const uint8_t val){return (value /= val);}\
	    inline uint8_t operator /= (const BOOL8 b8){return (value /= b8.value);}\
	    \
            inline uint8_t operator | (const uint8_t val){return (value | val);}\
            inline uint8_t operator | (const BOOL8 b8){return (value | b8.value);}\
	    inline uint8_t operator & (const uint8_t val){return (value & val);}\
	    inline uint8_t operator & (const BOOL8 b8){return (value & b8.value);}\
	    inline uint8_t operator ^ (const uint8_t val){return (value ^ val);}\
	    inline uint8_t operator ^ (const BOOL8 b8){return (value ^ b8.value);}\
	    inline uint8_t operator << (const uint8_t val){return (value << val);}\
	    inline uint8_t operator >> (const uint8_t val){return (value >> val);}\
	    inline uint8_t operator ~ (void){return (~value);}\
	    \
	    inline uint8_t operator |= (const uint8_t val){return (value |= val);}\
	    inline uint8_t operator |= (const BOOL8 b8){return (value |= b8.value);}\
	    inline uint8_t operator &= (const uint8_t val){return (value &= val);}\
	    inline uint8_t operator &= (const BOOL8 b8){return (value &= b8.value);}\
	    inline uint8_t operator ^= (const uint8_t val){return (value ^= val);}\
	    inline uint8_t operator ^= (const BOOL8 b8){return (value ^= b8.value);}\
	    \
            inline const uint8_t size() const{return BOOL8_BIT_SIZE;}\
            \
            \
    }




#define __SELECTION8_1(b0) __BOOL8_REDEF1(b0)
#define __SELECTION8_2(b0, b1) __BOOL8_REDEF2(b0 ,b1)
#define __SELECTION8_3(b0, b1, b2) __BOOL8_REDEF3(b0, b1, b2)
#define __SELECTION8_4(b0, b1, b2, b3) __BOOL8_REDEF4(b0, b1, b2, b3)
#define __SELECTION8_5(b0, b1, b2, b3, b4) __BOOL8_REDEF5(b0, b1, b2, b3, b4)
#define __SELECTION8_6(b0, b1, b2, b3, b4, b5) __BOOL8_REDEF6(b0, b1, b2, b3, b4, b5)
#define __SELECTION8_7(b0, b1, b2, b3, b4, b5, b6) __BOOL8_REDEF7(b0, b1, b2, b3, b4, b5, b6)
#define __SELECTION8_8(b0, b1, b2, b3, b4, b5, b6, b7) __BOOL8_REDEF8(b0, b1, b2, b3, b4, b5, b6, b7)

#define __SELECTION8_(N, ...) __SELECTION8_ ## N (__VA_ARGS__)
#define __SELECT8_(N, ...) __SELECTION8_(N, __VA_ARGS__)

#define BOOL8_REDEF(...) __SELECT8_(__NARGS(__VA_ARGS__), __VA_ARGS__)





/**********************************************************************
 *
 *  BOOL8_REDEF_PREF(...)
 *  Description:
 *  Rename b0...b7 with prefix of your choice
 *
 *  Usage:
 *  BOOL8_REDEF_PREF([prefix], [NAME0],...,[_NAME7]) [varName]
 *  varName.prefixNAME0=[true/false]
 *  ...
 *  varName.prefixNAME7=[true/false]
 *
 *  varName( [index] ) - return bit according to index
 *  varname[ [index] ] - equivalent to varName([index])
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL8
 *  varName.size(void) - return constant count of bits
 *  varName = [uint8_t] - overloaded operator will make easier
 *  to copy data from uint8_t
 *
 *  NOTE: macro is overloaded, you do not need
 *  to rename all 8 bits
 *
 **********************************************************************/

#define __BOOL8_REDEF_PREF0(_prefix) __BOOL8_REDEF_PREF8(_prefix,0,1,2,3,4,5,6,7)
#define __BOOL8_REDEF_PREF1(_prefix,b0) __BOOL8_REDEF_PREF8(_prefix,b0,1,2,3,4,5,6,7)
#define __BOOL8_REDEF_PREF2(_prefix,b0,b1) __BOOL8_REDEF_PREF8(_prefix,b0,b1,2,3,4,5,6,7)
#define __BOOL8_REDEF_PREF3(_prefix,b0,b1,b2) __BOOL8_REDEF_PREF8(_prefix,b0,b1,b2,3,4,5,6,7)
#define __BOOL8_REDEF_PREF4(_prefix,b0,b1,b2,b3) __BOOL8_REDEF_PREF8(_prefix,b0,b1,b2,b3,4,5,6,7)
#define __BOOL8_REDEF_PREF5(_prefix,b0,b1,b2,b3,b4) __BOOL8_REDEF_PREF8(_prefix,b0,b1,b2,b3,b4,5,6,7)
#define __BOOL8_REDEF_PREF6(_prefix,b0,b1,b2,b3,b4,b5) __BOOL8_REDEF_PREF8(_prefix,b0,b1,b2,b3,b4,b5,6,7)
#define __BOOL8_REDEF_PREF7(_prefix,b0,b1,b2,b3,b4,b5,b6) __BOOL8_REDEF_PREF8(_prefix,b0,b1,b2,b3,b4,b5,b6,7)

#define __BOOL8_REDEF_PREF8(_prefix,b0,b1,b2,b3,b4,b5,b6,b7) \
    struct{\
        union{\
            struct{\
		uint8_t _prefix##b0 :1;\
		uint8_t _prefix##b1 :1;\
		uint8_t _prefix##b2 :1;\
		uint8_t _prefix##b3 :1;\
		uint8_t _prefix##b4 :1;\
		uint8_t _prefix##b5 :1;\
		uint8_t _prefix##b6 :1;\
		uint8_t _prefix##b7 :1;\
            };\
            uint8_t value;\
        };\
            /**Dynamic Write*/\
            void operator ()(const uint8_t index, const bool bState){\
                value = (value & ( ~(0x01 << index ))) | (bState << index);\
            }\
    \
            /**Dynamic Read*/\
            inline bool operator () (const uint8_t index) const{\
                return (*this)[index];\
            }\
            /**Dynamic Read*/\
            bool operator [] (const uint8_t index) const{\
                return ((value >> index) & 0x01);\
            }\
    \
            inline uint8_t operator = (const uint8_t val){return value = val;}\
            inline uint8_t operator = (const BOOL8 b8){return value = b8.value;}\
            \
     	    inline uint8_t operator + (const uint8_t val){return (value + val);}\
	    inline uint8_t operator + (const BOOL8 b8){return (value + b8.value);}\
	    inline uint8_t operator - (const uint8_t val){return (value - val);}\
	    inline uint8_t operator - (const BOOL8 b8){return (value - b8.value);}\
	    inline uint8_t operator * (const uint8_t val){return (value * val);}\
	    inline uint8_t operator * (const BOOL8 b8){return (value * b8.value);}\
	    inline uint8_t operator / (const uint8_t val){return (value / val);}\
	    inline uint8_t operator / (const BOOL8 b8){return (value / b8.value);}\
	    \
	    inline uint8_t operator += (const uint8_t val){return (value += val);}\
	    inline uint8_t operator += (const BOOL8 b8){return (value += b8.value);}\
	    inline uint8_t operator -= (const uint8_t val){return (value -= val);}\
	    inline uint8_t operator -= (const BOOL8 b8){return (value -= b8.value);}\
	    inline uint8_t operator *= (const uint8_t val){return (value *= val);}\
	    inline uint8_t operator *= (const BOOL8 b8){return (value *= b8.value);}\
	    inline uint8_t operator /= (const uint8_t val){return (value /= val);}\
	    inline uint8_t operator /= (const BOOL8 b8){return (value /= b8.value);}\
	    \
            inline uint8_t operator | (const uint8_t val){return (value | val);}\
	    inline uint8_t operator | (const BOOL8 b8){return (value | b8.value);}\
	    inline uint8_t operator & (const uint8_t val){return (value & val);}\
	    inline uint8_t operator & (const BOOL8 b8){return (value & b8.value);}\
	    inline uint8_t operator ^ (const uint8_t val){return (value ^ val);}\
	    inline uint8_t operator ^ (const BOOL8 b8){return (value ^ b8.value);}\
	    inline uint8_t operator << (const uint8_t val){return (value << val);}\
	    inline uint8_t operator >> (const uint8_t val){return (value >> val);}\
	    inline uint8_t operator ~ (void){return (~value);}\
	    \
	    inline uint8_t operator |= (const uint8_t val){return (value |= val);}\
	    inline uint8_t operator |= (const BOOL8 b8){return (value |= b8.value);}\
	    inline uint8_t operator &= (const uint8_t val){return (value &= val);}\
	    inline uint8_t operator &= (const BOOL8 b8){return (value &= b8.value);}\
	    inline uint8_t operator ^= (const uint8_t val){return (value ^= val);}\
	    inline uint8_t operator ^= (const BOOL8 b8){return (value ^= b8.value);}\
    	    \
            inline const uint8_t size() const{return BOOL8_BIT_SIZE;}\
            \
            \
    }




#define __SELECTION_PREF8_1(_prefix) __BOOL8_REDEF_PREF0(_prefix)
#define __SELECTION_PREF8_2(_prefix,b0) __BOOL8_REDEF_PREF1(_prefix,b0)
#define __SELECTION_PREF8_3(_prefix,b0, b1) __BOOL8_REDEF_PREF2(_prefix,b0, b1)
#define __SELECTION_PREF8_4(_prefix,b0, b1, b2) __BOOL8_REDEF_PREF3(_prefix,b0, b1, b2)
#define __SELECTION_PREF8_5(_prefix,b0, b1, b2, b3) __BOOL8_REDEF_PREF4(_prefix,b0, b1, b2, b3)
#define __SELECTION_PREF8_6(_prefix,b0, b1, b2, b3, b4) __BOOL8_REDEF_PREF5(_prefix,b0, b1, b2, b3, b4)
#define __SELECTION_PREF8_7(_prefix,b0, b1, b2, b3, b4, b5) __BOOL8_REDEF_PREF6(_prefix,b0, b1, b2, b3, b4, b5)
#define __SELECTION_PREF8_8(_prefix,b0, b1, b2, b3, b4, b5, b6) __BOOL8_REDEF_PREF7(_prefix,b0, b1, b2, b3, b4, b5, b6)
#define __SELECTION_PREF8_9(_prefix,b0, b1, b2, b3, b4, b5, b6, b7) __BOOL8_REDEF_PREF8(_prefix,b0, b1, b2, b3, b4, b5, b6, b7)

#define __SELECTION_PREF8(N, ...) __SELECTION_PREF8_ ## N (__VA_ARGS__)
#define SELECT_PREF8(N, ...) __SELECTION_PREF8(N, __VA_ARGS__)

#define BOOL8_REDEF_PREFIX(...) SELECT_PREF8(__NARGS(__VA_ARGS__), __VA_ARGS__)







#define BOOL16_BIT_SIZE 16
#define BOOL16_BYTE_SIZE (BOOL8_BIT_SIZE / 8)





/******************************************************************//**
 *
 *  BOOL16
 *  Description:
 *
 *  Usage:
 *
 *  BOOL16 [varName]
 *
 *  BOOL16 [varName] (b0,...,b7)
 *
 *  varName.b0=[true/false]
 *
 *  ...
 *
 *  varName.b15=[true/false]
 *
 *  varName( [index] ) - return bit according to index
 *
 *  varname[ [index] ] - equivalent to varName([index])
 *
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *
 *  varName( [index] ) - return bit according to index
 *
 *  varname[ [index] ] - equivalent to varName([index])
 *
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL16
 *
 *  varName.size(void) - return constant count of bits
 *
 *  varName = [uint16_t] - overloaded operator will make easier
 *  to copy data from uint16_ts
 *
 **********************************************************************/

struct BOOL16{
    union{
        struct{
             uint8_t b0 :1;
             uint8_t b1 :1;
             uint8_t b2 :1;
             uint8_t b3 :1;
             uint8_t b4 :1;
             uint8_t b5 :1;
             uint8_t b6 :1;
             uint8_t b7 :1;
             uint8_t b8 :1;
             uint8_t b9 :1;
             uint8_t b10 :1;
             uint8_t b11 :1;
             uint8_t b12 :1;
             uint8_t b13 :1;
             uint8_t b14 :1;
             uint8_t b15 :1;
    	};
    	uint16_t value;
    	
    };
    BOOL16()=default;
    BOOL16(const bool b0, const bool b1 = 0, const bool b2 = 0, const bool b3 = 0, const bool b4 = 0,
        const bool b5 = 0, const bool b6 = 0, const bool b7 = 0,
        const bool b8 = 0, const bool b9 = 0, const bool b10 = 0, const bool b11 = 0, const bool b12 = 0,
        const bool b13 = 0, const bool b14 = 0, const bool b15 = 0)
        : b0(b0) ,b1(b1), b2(b2), b3(b3), b4(b4), b5(b5), b6(b6), b7(b7)
        , b8(b8) ,b9(b9), b10(b10), b11(b11), b12(b12), b13(b13), b14(b14), b15(b15){}

    /**Dynamic Write*/
    void operator ()(const uint8_t index, const bool bState){
        value = (value & ( ~(0x01 << index))) |(bState << index);
    }

    /**Dynamic Read*/
    inline bool operator () (const uint8_t index) const{
        return (*this)[index];
    }
    /**Dynamic Read*/
    bool operator [] (const uint8_t index) const{
        return ((value >> index) & 0x01);
    }

    inline uint16_t operator = (const uint16_t val){return value = val;}
    
    inline uint16_t operator + (const uint16_t val){return (value + val);}
    inline uint16_t operator + (const BOOL16 b16){return (value + b16.value);}
    inline uint16_t operator - (const uint16_t val){return (value - val);}
    inline uint16_t operator - (const BOOL16 b16){return (value - b16.value);}
    inline uint16_t operator * (const uint16_t val){return (value * val);}
    inline uint16_t operator * (const BOOL16 b16){return (value * b16.value);}
    inline uint16_t operator / (const uint16_t val){return (value / val);}
    inline uint16_t operator / (const BOOL16 b16){return (value / b16.value);}
    
    inline uint16_t operator += (const uint16_t val){return (value += val);}
    inline uint16_t operator += (const BOOL16 b16){return (value += b16.value);}
    inline uint16_t operator -= (const uint16_t val){return (value -= val);}
    inline uint16_t operator -= (const BOOL16 b16){return (value -= b16.value);}
    inline uint16_t operator *= (const uint16_t val){return (value *= val);}
    inline uint16_t operator *= (const BOOL16 b16){return (value *= b16.value);}
    inline uint16_t operator /= (const uint16_t val){return (value /= val);}
    inline uint16_t operator /= (const BOOL16 b16){return (value /= b16.value);}
    
    inline uint16_t operator | (const uint16_t val){return (value | val);}
    inline uint16_t operator | (const BOOL16 b16){return (value | b16.value);}
    inline uint16_t operator & (const uint16_t val){return (value & val);}
    inline uint16_t operator & (const BOOL16 b16){return (value & b16.value);}
    inline uint16_t operator ^ (const uint16_t val){return (value ^ val);}
    inline uint16_t operator ^ (const BOOL16 b16){return (value ^ b16.value);}
    inline uint16_t operator << (const uint16_t val){return (value << val);}
    inline uint16_t operator >> (const uint16_t val){return (value >> val);}
    inline uint16_t operator ~ (void){return (~value);}
    
    inline uint16_t operator |= (const uint16_t val){return (value |= val);}
    inline uint16_t operator |= (const BOOL16 b16){return (value |= b16.value);}
    inline uint16_t operator &= (const uint16_t val){return (value &= val);}
    inline uint16_t operator &= (const BOOL16 b16){return (value &= b16.value);}
    inline uint16_t operator ^= (const uint16_t val){return (value ^= val);}
    inline uint16_t operator ^= (const BOOL16 b16){return (value ^= b16.value);}
    
    inline const uint8_t size() const{return BOOL16_BIT_SIZE;}

};



/**********************************************************************
 *
 *  BOOL16_REDEF(...)
 *  Description:
 *  Rename b0...b15 with prefix 'b'
 *
 *  Usage:
 *  BOOL16_REDEF([NAME0],...,[NAME15]) [varName]
 *  varName.bNAME0=[true/false]
 *  ...
 *  varName.bNAME15=[true/false]
 *
 *  varName([index]) - return bit according to index
 *  varname[[index]] - equivalent to varName([index])
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL16
 *  varName.size(void) - return constant count of bits
 *  varName = [uint16_t] - overloaded operator will make easier
 *  to copy data from uint16_ts
 *
 *  NOTE: macro is overloaded, you do not need
 *  to rename all 16 bits
 *
 **********************************************************************/

#define __BOOL16_REDEF1(b0) __BOOL16_REDEF16(b0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF2(b0,b1) __BOOL16_REDEF16(b0,b1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF3(b0,b1,b2) __BOOL16_REDEF16(b0,b1,b2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF4(b0,b1,b2,b3) __BOOL16_REDEF16(b0,b1,b2,b3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF5(b0,b1,b2,b3,b4) __BOOL16_REDEF16(b0,b1,b2,b3,b4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF6(b0,b1,b2,b3,b4,b5) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF7(b0,b1,b2,b3,b4,b5,b6) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF8(b0,b1,b2,b3,b4,b5,b6,b7) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF9(b0,b1,b2,b3,b4,b5,b6,b7,b8) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF10(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,10,11,12,13,14,15)
#define __BOOL16_REDEF11(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,11,12,13,14,15)
#define __BOOL16_REDEF12(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,12,13,14,15)
#define __BOOL16_REDEF13(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,13,14,15)
#define __BOOL16_REDEF14(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,14,15)
#define __BOOL16_REDEF15(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14) __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,15)

#define __BOOL16_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15) \
    struct{\
        union{\
            struct{\
                 uint8_t b##b0 :1;\
                 uint8_t b##b1 :1;\
                 uint8_t b##b2 :1;\
                 uint8_t b##b3 :1;\
                 uint8_t b##b4 :1;\
                 uint8_t b##b5 :1;\
                 uint8_t b##b6 :1;\
                 uint8_t b##b7 :1;\
                 uint8_t b##b8 :1;\
                 uint8_t b##b9 :1;\
                 uint8_t b##b10 :1;\
                 uint8_t b##b11 :1;\
                 uint8_t b##b12 :1;\
                 uint8_t b##b13 :1;\
                 uint8_t b##b14 :1;\
                 uint8_t b##b15 :1;\
            };\
            uint16_t value;\
       };\
            /**Dynamic Write*/\
            void operator ()(const uint8_t index, const bool bState){\
                value = (value & ( ~(0x01 << index))) | (bState << index);\
            }\
    \
            /**Dynamic Read*/\
            inline bool operator () (const uint8_t index) const{\
                return (*this)[index];\
            }\
            /**Dynamic Read*/\
            bool operator [] (const uint8_t index) const{\
                return ((value >> index) & 0x01);\
            }\
    \
            inline uint16_t operator = (const uint16_t val){return value = val;}\
            inline uint16_t operator = (const BOOL16 b16){return value = b16.value;}\
            \
            inline uint16_t operator + (const uint16_t val){return (value + val);}\
	    inline uint16_t operator + (const BOOL16 b16){return (value + b16.value);}\
	    inline uint16_t operator - (const uint16_t val){return (value - val);}\
	    inline uint16_t operator - (const BOOL16 b16){return (value - b16.value);}\
	    inline uint16_t operator * (const uint16_t val){return (value * val);}\
	    inline uint16_t operator * (const BOOL16 b16){return (value * b16.value);}\
	    inline uint16_t operator / (const uint16_t val){return (value / val);}\
	    inline uint16_t operator / (const BOOL16 b16){return (value / b16.value);}\
	    \
	    inline uint16_t operator += (const uint16_t val){return (value += val);}\
	    inline uint16_t operator += (const BOOL16 b16){return (value += b16.value);}\
	    inline uint16_t operator -= (const uint16_t val){return (value -= val);}\
	    inline uint16_t operator -= (const BOOL16 b16){return (value -= b16.value);}\
	    inline uint16_t operator *= (const uint16_t val){return (value *= val);}\
	    inline uint16_t operator *= (const BOOL16 b16){return (value *= b16.value);}\
	    inline uint16_t operator /= (const uint16_t val){return (value /= val);}\
	    inline uint16_t operator /= (const BOOL16 b16){return (value /= b16.value);}\
	    \
	    inline uint16_t operator | (const uint16_t val){return (value | val);}\
	    inline uint16_t operator | (const BOOL16 b16){return (value | b16.value);}\
	    inline uint16_t operator & (const uint16_t val){return (value & val);}\
	    inline uint16_t operator & (const BOOL16 b16){return (value & b16.value);}\
	    inline uint16_t operator ^ (const uint16_t val){return (value ^ val);}\
	    inline uint16_t operator ^ (const BOOL16 b16){return (value ^ b16.value);}\
	    inline uint16_t operator << (const uint16_t val){return (value << val);}\
	    inline uint16_t operator >> (const uint16_t val){return (value >> val);}\
    	    inline uint16_t operator ~ (void){return (~value);}\
            \
            inline uint16_t operator |= (const uint16_t val){return (value |= val);}\
	    inline uint16_t operator |= (const BOOL16 b16){return (value |= b16.value);}\
	    inline uint16_t operator &= (const uint16_t val){return (value &= val);}\
	    inline uint16_t operator &= (const BOOL16 b16){return (value &= b16.value);}\
	    inline uint16_t operator ^= (const uint16_t val){return (value ^= val);}\
	    inline uint16_t operator ^= (const BOOL16 b16){return (value ^= b16.value);}\
            inline const uint8_t size() const{return BOOL16_BIT_SIZE;}\
            \
    }




#define __SELECTION16_1(b0) __BOOL16_REDEF1(b0)
#define __SELECTION16_2(b0, b1) __BOOL16_REDEF2(b0 ,b1)
#define __SELECTION16_3(b0, b1, b2) __BOOL16_REDEF3(b0, b1, b2)
#define __SELECTION16_4(b0, b1, b2, b3) __BOOL16_REDEF4(b0, b1, b2, b3)
#define __SELECTION16_5(b0, b1, b2, b3, b4) __BOOL16_REDEF5(b0, b1, b2, b3, b4)
#define __SELECTION16_6(b0, b1, b2, b3, b4, b5) __BOOL16_REDEF6(b0, b1, b2, b3, b4, b5)
#define __SELECTION16_7(b0, b1, b2, b3, b4, b5, b6) __BOOL16_REDEF7(b0, b1, b2, b3, b4, b5, b6)
#define __SELECTION16_8(b0, b1, b2, b3, b4, b5, b6, b7) __BOOL16_REDEF8(b0, b1, b2, b3, b4, b5, b6, b7)
#define __SELECTION16_9(b0, b1, b2, b3, b4, b5, b6, b7, b8) __BOOL16_REDEF9(b0, b1, b2, b3, b4, b5, b6, b7, b8)
#define __SELECTION16_10(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9) __BOOL16_REDEF10(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9)
#define __SELECTION16_11(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10) __BOOL16_REDEF11(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10)
#define __SELECTION16_12(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11) __BOOL16_REDEF12(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11)
#define __SELECTION16_13(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12) __BOOL16_REDEF13(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12)
#define __SELECTION16_14(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13) __BOOL16_REDEF14(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13)
#define __SELECTION16_15(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14) __BOOL16_REDEF15(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14)
#define __SELECTION16_16(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15) __BOOL16_REDEF16(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15)

#define __SELECTION16_(N, ...) __SELECTION16_ ## N (__VA_ARGS__)
#define __SELECT16_(N, ...) __SELECTION16_(N, __VA_ARGS__)

#define BOOL16_REDEF(...) __SELECT16_(__NARGS(__VA_ARGS__), __VA_ARGS__)







/**********************************************************************
 *
 *  BOOL16_REDEF_PREF(...)
 *  Description:
 *  Rename b0...b15 with prefix of your choice
 *
 *  Usage:
 *  BOOL16_REDEF_PREF([prefix], [NAME0],...,[NAME15]) [varName]
 *  varName.prefixNAME0=[true/false]
 *  ...
 *  varName.prefixNAME15=[true/false]
 *
 *  varName( [index] ) - return bit according to index
 *  varname[ [index] ] - equivalent to varName([index])
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL16
 *  varName.size(void) - return constant count of bits
 *  varName = [uint16_t] - overloaded operator will make easier
 *  to copy data from uint16_ts
 *
 *  NOTE: macro is overloaded, you do not need
 *  to rename all 16 bits
 *
 **********************************************************************/

#define __BOOL16_REDEF_PREF0(_prefix) __BOOL16_REDEF_PREF16(_prefix,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF1(_prefix,b0) __BOOL16_REDEF_PREF16(_prefix,b0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF2(_prefix,b0,b1) __BOOL16_REDEF_PREF16(_prefix,b0,b1,2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF3(_prefix,b0,b1,b2) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF4(_prefix,b0,b1,b2,b3) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF5(_prefix,b0,b1,b2,b3,b4) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF6(_prefix,b0,b1,b2,b3,b4,b5) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF7(_prefix,b0,b1,b2,b3,b4,b5,b6) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF8(_prefix,b0,b1,b2,b3,b4,b5,b6,b7) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF9(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF10(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF11(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,11,12,13,14,15)
#define __BOOL16_REDEF_PREF12(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,12,13,14,15)
#define __BOOL16_REDEF_PREF13(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,13,14,15)
#define __BOOL16_REDEF_PREF14(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,14,15)
#define __BOOL16_REDEF_PREF15(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14) __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,15)

#define __BOOL16_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15) \
    struct{\
        union{\
            struct{\
                 uint8_t _prefix##b0 :1;\
                 uint8_t _prefix##b1 :1;\
                 uint8_t _prefix##b2 :1;\
                 uint8_t _prefix##b3 :1;\
                 uint8_t _prefix##b4 :1;\
                 uint8_t _prefix##b5 :1;\
                 uint8_t _prefix##b6 :1;\
                 uint8_t _prefix##b7 :1;\
                 uint8_t _prefix##b8 :1;\
                 uint8_t _prefix##b9 :1;\
                 uint8_t _prefix##b10 :1;\
                 uint8_t _prefix##b11 :1;\
                 uint8_t _prefix##b12 :1;\
                 uint8_t _prefix##b13 :1;\
                 uint8_t _prefix##b14 :1;\
                 uint8_t _prefix##b15 :1;\
            };\
            uint16_t value;\
         };\
            /**Dynamic Write*/\
            void operator ()(const uint8_t index, const bool bState){\
                value = (value & ( ~(0x01 << index))) | (bState << index);\
            }\
    \
            /**Dynamic Read*/\
            inline bool operator () (const uint8_t index) const{\
                return (*this)[index];\
            }\
            /**Dynamic Read*/\
            bool operator [] (const uint8_t index) const{\
                return ((value >> index) & 0x01);\
            }\
    \
            inline uint16_t operator = (const uint16_t val){return value = val;}\
            inline uint16_t operator = (const BOOL16 b16){return value = b16.value;}\
            \
            inline uint16_t operator + (const uint16_t val){return (value + val);}\
	    inline uint16_t operator + (const BOOL16 b16){return (value + b16.value);}\
	    inline uint16_t operator - (const uint16_t val){return (value - val);}\
	    inline uint16_t operator - (const BOOL16 b16){return (value - b16.value);}\
	    inline uint16_t operator * (const uint16_t val){return (value * val);}\
	    inline uint16_t operator * (const BOOL16 b16){return (value * b16.value);}\
	    inline uint16_t operator / (const uint16_t val){return (value / val);}\
	    inline uint16_t operator / (const BOOL16 b16){return (value / b16.value);}\
	    \
	    inline uint16_t operator += (const uint16_t val){return (value += val);}\
	    inline uint16_t operator += (const BOOL16 b16){return (value += b16.value);}\
	    inline uint16_t operator -= (const uint16_t val){return (value -= val);}\
	    inline uint16_t operator -= (const BOOL16 b16){return (value -= b16.value);}\
	    inline uint16_t operator *= (const uint16_t val){return (value *= val);}\
	    inline uint16_t operator *= (const BOOL16 b16){return (value *= b16.value);}\
	    inline uint16_t operator /= (const uint16_t val){return (value /= val);}\
	    inline uint16_t operator /= (const BOOL16 b16){return (value /= b16.value);}\
	    \
            inline uint16_t operator | (const uint16_t val){return (value | val);}\
	    inline uint16_t operator | (const BOOL16 b16){return (value | b16.value);}\
	    inline uint16_t operator & (const uint16_t val){return (value & val);}\
	    inline uint16_t operator & (const BOOL16 b16){return (value & b16.value);}\
	    inline uint16_t operator ^ (const uint16_t val){return (value ^ val);}\
	    inline uint16_t operator ^ (const BOOL16 b16){return (value ^ b16.value);}\
	    inline uint16_t operator << (const uint16_t val){return (value << val);}\
	    inline uint16_t operator >> (const uint16_t val){return (value >> val);}\
    	    inline uint16_t operator ~ (void){return (~value);}\
            \
            inline uint16_t operator |= (const uint16_t val){return (value |= val);}\
	    inline uint16_t operator |= (const BOOL16 b16){return (value |= b16.value);}\
	    inline uint16_t operator &= (const uint16_t val){return (value &= val);}\
	    inline uint16_t operator &= (const BOOL16 b16){return (value &= b16.value);}\
	    inline uint16_t operator ^= (const uint16_t val){return (value ^= val);}\
	    inline uint16_t operator ^= (const BOOL16 b16){return (value ^= b16.value);}\
            inline const uint8_t size() const{return BOOL16_BIT_SIZE;}\
            \
            \
    }




#define __SELECTION_PREF16_1(_prefix) __BOOL16_REDEF_PREF0(_prefix)
#define __SELECTION_PREF16_2(_prefix,b0) __BOOL16_REDEF_PREF1(_prefix,b0)
#define __SELECTION_PREF16_3(_prefix,b0, b1) __BOOL16_REDEF_PREF2(_prefix,b0, b1)
#define __SELECTION_PREF16_4(_prefix,b0, b1, b2) __BOOL16_REDEF_PREF3(_prefix,b0, b1, b2)
#define __SELECTION_PREF16_5(_prefix,b0, b1, b2, b3) __BOOL16_REDEF_PREF4(_prefix,b0, b1, b2, b3)
#define __SELECTION_PREF16_6(_prefix,b0, b1, b2, b3, b4) __BOOL16_REDEF_PREF5(_prefix,b0, b1, b2, b3, b4)
#define __SELECTION_PREF16_7(_prefix,b0, b1, b2, b3, b4, b5) __BOOL16_REDEF_PREF6(_prefix,b0, b1, b2, b3, b4, b5)
#define __SELECTION_PREF16_8(_prefix,b0, b1, b2, b3, b4, b5, b6) __BOOL16_REDEF_PREF7(_prefix,b0, b1, b2, b3, b4, b5, b6)
#define __SELECTION_PREF16_9(_prefix,b0, b1, b2, b3, b4, b5, b6, b7) __BOOL16_REDEF_PREF8(_prefix,b0, b1, b2, b3, b4, b5, b6, b7)
#define __SELECTION_PREF16_10(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8) __BOOL16_REDEF_PREF9(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8)
#define __SELECTION_PREF16_11(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9) __BOOL16_REDEF_PREF10(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9)
#define __SELECTION_PREF16_12(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10) __BOOL16_REDEF_PREF11(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10)
#define __SELECTION_PREF16_13(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11) __BOOL16_REDEF_PREF12(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11)
#define __SELECTION_PREF16_14(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12) __BOOL16_REDEF_PREF13(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12)
#define __SELECTION_PREF16_15(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13) __BOOL16_REDEF_PREF14(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13)
#define __SELECTION_PREF16_16(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14) __BOOL16_REDEF_PREF15(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14)
#define __SELECTION_PREF16_17(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15) __BOOL16_REDEF_PREF16(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15)


#define __SELECTION_PREF16(N, ...) __SELECTION_PREF16_ ## N (__VA_ARGS__)
#define SELECT_PREF16(N, ...) __SELECTION_PREF16(N, __VA_ARGS__)

#define BOOL16_REDEF_PREFIX(...) SELECT_PREF16(__NARGS(__VA_ARGS__), __VA_ARGS__)
#define BOOL16_REDEF_PREF(...) SELECT_PREF16(__NARGS(__VA_ARGS__), __VA_ARGS__) 






#define BOOL32_BIT_SIZE 32
#define BOOL32_BYTE_SIZE (BOOL8_BIT_SIZE / 8)


/**********************************************************************
 *
 *  BOOL32(...)
 *  Description:
 *  
 *
 *  Usage:
 *  BOOL32 [varName]
 *  varName.b0=[true/false]
 *  ...
 *  varName.b31=[true/false]
 *
 *  varName( [index] ) - return bit according to index
 *  varname[ [index] ] - equivalent to varName([index])
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL32
 *  varName.size(void) - return constant count of bits
 *  varName = [unsigned int] - overloaded operator will make easier
 *  to copy data from unsigned int
 *
 **********************************************************************/

struct BOOL32{
    union{
        struct{
            uint8_t b0 :1;
            uint8_t b1 :1;
            uint8_t b2 :1;
            uint8_t b3 :1;
            uint8_t b4 :1;
            uint8_t b5 :1;
            uint8_t b6 :1;
            uint8_t b7 :1;
            uint8_t b8 :1;
            uint8_t b9 :1;
            uint8_t b10 :1;
            uint8_t b11 :1;
            uint8_t b12 :1;
            uint8_t b13 :1;
            uint8_t b14 :1;
            uint8_t b15 :1;
            uint8_t b16 :1;
            uint8_t b17 :1;
            uint8_t b18 :1;
            uint8_t b19 :1;
            uint8_t b20 :1;
            uint8_t b21 :1;
            uint8_t b22 :1;
            uint8_t b23 :1;
            uint8_t b24 :1;
            uint8_t b25 :1;
            uint8_t b26 :1;
            uint8_t b27 :1;
            uint8_t b28 :1;
            uint8_t b29 :1;
            uint8_t b30 :1;
            uint8_t b31 :1;
         };
         uint32_t value;
      };

    BOOL32()=default;
    BOOL32(const bool b0, const bool b1 = 0, const bool b2 = 0, const bool b3 = 0, const bool b4 = 0,
        const bool b5 = 0, const bool b6 = 0, const bool b7 = 0,
        const bool b8 = 0, const bool b9 = 0, const bool b10 = 0, const bool b11 = 0, const bool b12 = 0,
        const bool b13 = 0, const bool b14 = 0, const bool b15 = 0,
        const bool b16 = 0, const bool b17 = 0, const bool b18 = 0, const bool b19 = 0, const bool b20 = 0,
        const bool b21 = 0, const bool b22 = 0, const bool b23 = 0,
        const bool b24 = 0, const bool b25 = 0, const bool b26 = 0, const bool b27 = 0, const bool b28 = 0,
        const bool b29 = 0, const bool b30 = 0, const bool b31 = 0)
        : b0(b0) ,b1(b1), b2(b2), b3(b3), b4(b4), b5(b5), b6(b6), b7(b7)
        , b8(b8) ,b9(b9), b10(b10), b11(b11), b12(b12), b13(b13), b14(b14), b15(b15)
        , b16(b16) ,b17(b17), b18(b18), b19(b19), b20(b20), b21(b21), b22(b22), b23(b23)
        , b24(b24) ,b25(b25), b26(b26), b27(b27), b28(b28), b29(b29), b30(b30), b31(b31){}

    /**Dynamic Write*/
    void operator ()(const uint8_t index, const bool bState){
        value = (value & ( ~(0x01 << index))) | (bState << index);
    }

    /**Dynamic Read*/
    inline bool operator () (const uint8_t index) const{
        return (*this)[index];
    }
    /**Dynamic Read*/
    bool operator [] (const uint8_t index) const{
        return ((value >> index) & 0x01);
    }

    inline uint32_t operator = (const uint32_t val){return value = val;}
    
    inline uint32_t operator + (const uint32_t val){return (value + val);}
    inline uint32_t operator + (const BOOL32 b32){return (value + b32.value);}
    inline uint32_t operator - (const uint32_t val){return (value - val);}
    inline uint32_t operator - (const BOOL32 b32){return (value - b32.value);}
    inline uint32_t operator * (const uint32_t val){return (value * val);}
    inline uint32_t operator * (const BOOL32 b32){return (value * b32.value);}
    inline uint32_t operator / (const uint32_t val){return (value / val);}
    inline uint32_t operator / (const BOOL32 b32){return (value / b32.value);}
    
    inline uint32_t operator += (const uint32_t val){return (value += val);}
    inline uint32_t operator += (const BOOL32 b32){return (value += b32.value);}
    inline uint32_t operator -= (const uint32_t val){return (value -= val);}
    inline uint32_t operator -= (const BOOL32 b32){return (value -= b32.value);}
    inline uint32_t operator *= (const uint32_t val){return (value *= val);}
    inline uint32_t operator *= (const BOOL32 b32){return (value *= b32.value);}
    inline uint32_t operator /= (const uint32_t val){return (value /= val);}
    inline uint32_t operator /= (const BOOL32 b32){return (value /= b32.value);}
    			
    inline uint32_t operator | (const uint32_t val){return (value | val);}
    inline uint32_t operator | (const BOOL32 b32){return (value | b32.value);}
    inline uint32_t operator & (const uint32_t val){return (value & val);}
    inline uint32_t operator & (const BOOL32 b32){return (value & b32.value);}
    inline uint32_t operator ^ (const uint32_t val){return (value ^ val);}
    inline uint32_t operator ^ (const BOOL32 b32){return (value ^ b32.value);}
    inline uint32_t operator << (const uint32_t val){return (value << val);}
    inline uint32_t operator >> (const uint32_t val){return (value >> val);}
    inline uint32_t operator ~ (void){return (~value);}
    
    inline uint32_t operator |= (const uint32_t val){return (value |= val);}
    inline uint32_t operator |= (const BOOL32 b32){return (value |= b32.value);}
    inline uint32_t operator &= (const uint32_t val){return (value &= val);}
    inline uint32_t operator &= (const BOOL32 b32){return (value &= b32.value);}
    inline uint32_t operator ^= (const uint32_t val){return (value ^= val);}
    inline uint32_t operator ^= (const BOOL32 b32){return (value ^= b32.value);}
    
    inline const uint8_t size() const{return BOOL32_BIT_SIZE;}

};




/**********************************************************************
 *
 *  BOOL32_REDEF(...)
 *  Description:
 *  Rename b0...b31 with prefix 'b'
 *
 *  Usage:
 *  BOOL32_REDEF([NAME0],...,[NAME31]) [varName]
 *  varName.bNAME0=[true/false]
 *  ...
 *  varName.bNAME31=[true/false]
 *
 *  varName([index]) - return bit according to index
 *  varname[[index]] - equivalent to varName([index])
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *  varName.getValue(void) - returns unsigned int representation
 *  of bits in BOOL32
 *  varName.size(void) - return constant count of bits
 *  varName = [unsigned int] - overloaded operator will make easier
 *  to copy data from unsigned int
 *
 *  NOTE: macro is overloaded, you do not need
 *  to rename all 32 bits
 *
 **********************************************************************/


#define __BOOL32_REDEF1(b0) __BOOL32_REDEF32(b0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF2(b0,b1) __BOOL32_REDEF32(b0,b1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF3(b0,b1,b2) __BOOL32_REDEF32(b0,b1,b2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF4(b0,b1,b2,b3) __BOOL32_REDEF32(b0,b1,b2,b3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF5(b0,b1,b2,b3,b4) __BOOL32_REDEF32(b0,b1,b2,b3,b4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF6(b0,b1,b2,b3,b4,b5) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF7(b0,b1,b2,b3,b4,b5,b6) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF8(b0,b1,b2,b3,b4,b5,b6,b7) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF9(b0,b1,b2,b3,b4,b5,b6,b7,b8) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF10(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF11(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF12(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF13(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF14(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF15(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF16(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF17(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF18(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF19(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF20(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF21(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF22(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF23(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF24(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF25(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF26(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,26,27,28,29,30,31)
#define __BOOL32_REDEF27(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,27,28,29,30,31)
#define __BOOL32_REDEF28(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,28,29,30,31)
#define __BOOL32_REDEF29(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,29,30,31)
#define __BOOL32_REDEF30(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,30,31)
#define __BOOL32_REDEF31(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,b30) __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,b30,31)

#define __BOOL32_REDEF32(b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15, b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,b30,b31) \
    struct{\
        union{\
            struct{\
                uint8_t b##b0 :1;\
                uint8_t b##b1 :1;\
                uint8_t b##b2 :1;\
                uint8_t b##b3 :1;\
                uint8_t b##b4 :1;\
                uint8_t b##b5 :1;\
                uint8_t b##b6 :1;\
                uint8_t b##b7 :1;\
                uint8_t b##b8 :1;\
                uint8_t b##b9 :1;\
                uint8_t b##b10 :1;\
                uint8_t b##b11 :1;\
                uint8_t b##b12 :1;\
                uint8_t b##b13 :1;\
                uint8_t b##b14 :1;\
                uint8_t b##b15 :1;\
                uint8_t b##b16 :1;\
                uint8_t b##b17 :1;\
                uint8_t b##b18 :1;\
                uint8_t b##b19 :1;\
                uint8_t b##b20 :1;\
                uint8_t b##b21 :1;\
                uint8_t b##b22 :1;\
                uint8_t b##b23 :1;\
                uint8_t b##b24 :1;\
                uint8_t b##b25 :1;\
                uint8_t b##b26 :1;\
                uint8_t b##b27 :1;\
                uint8_t b##b28 :1;\
                uint8_t b##b29 :1;\
                uint8_t b##b30 :1;\
                uint8_t b##b31 :1;\
            };\
            uint32_t value;\
        };\
            /**Dynamic Write*/\
            void operator ()(const uint8_t index, const bool bState){\
                value = (value & ( ~(0x01 << index))) | (bState << index);\
            }\
    \
            /**Dynamic Read*/\
            inline bool operator () (const uint8_t index) const{\
                return (*this)[index];\
            }\
            /**Dynamic Read*/\
            bool operator [] (const uint8_t index) const{\
                return ((value >> index) & 0x01);\
            }\
    \
            inline uint32_t operator = (const uint32_t val){return value = val;}\
            inline uint32_t operator = (const BOOL32 b32){return value = b32.value;}\
            \
            inline uint32_t operator + (const uint32_t val){return (value + val);}\
	    inline uint32_t operator + (const BOOL32 b32){return (value + b32.value);}\
	    inline uint32_t operator - (const uint32_t val){return (value - val);}\
	    inline uint32_t operator - (const BOOL32 b32){return (value - b32.value);}\
	    inline uint32_t operator * (const uint32_t val){return (value * val);}\
	    inline uint32_t operator * (const BOOL32 b32){return (value * b32.value);}\
	    inline uint32_t operator / (const uint32_t val){return (value / val);}\
	    inline uint32_t operator / (const BOOL32 b32){return (value / b32.value);}\
	    \
	    inline uint32_t operator += (const uint32_t val){return (value += val);}\
	    inline uint32_t operator += (const BOOL32 b32){return (value += b32.value);}\
	    inline uint32_t operator -= (const uint32_t val){return (value -= val);}\
	    inline uint32_t operator -= (const BOOL32 b32){return (value -= b32.value);}\
	    inline uint32_t operator *= (const uint32_t val){return (value *= val);}\
	    inline uint32_t operator *= (const BOOL32 b32){return (value *= b32.value);}\
	    inline uint32_t operator /= (const uint32_t val){return (value /= val);}\
	    inline uint32_t operator /= (const BOOL32 b32){return (value /= b32.value);}\
    	    \
            inline uint32_t operator | (const uint32_t val){return (value | val);}\
	    inline uint32_t operator | (const BOOL32 b32){return (value | b32.value);}\
	    inline uint32_t operator & (const uint32_t val){return (value & val);}\
	    inline uint32_t operator & (const BOOL32 b32){return (value & b32.value);}\
	    inline uint32_t operator ^ (const uint32_t val){return (value ^ val);}\
	    inline uint32_t operator ^ (const BOOL32 b32){return (value ^ b32.value);}\
	    inline uint32_t operator << (const uint32_t val){return (value << val);}\
	    inline uint32_t operator >> (const uint32_t val){return (value >> val);}\
	    inline uint32_t operator ~ (void){return (~value);}\
	    \
	    inline uint32_t operator |= (const uint32_t val){return (value |= val);}\
	    inline uint32_t operator |= (const BOOL32 b32){return (value |= b32.value);}\
	    inline uint32_t operator &= (const uint32_t val){return (value &= val);}\
	    inline uint32_t operator &= (const BOOL32 b32){return (value &= b32.value);}\
	    inline uint32_t operator ^= (const uint32_t val){return (value ^= val);}\
	    inline uint32_t operator ^= (const BOOL32 b32){return (value ^= b32.value);}\
	    \
            inline const uint8_t size() const{return BOOL32_BIT_SIZE;}\
            \
            \
    }




#define __SELECTION32_1(b0) __BOOL32_REDEF1(b0)
#define __SELECTION32_2(b0, b1) __BOOL32_REDEF2(b0 ,b1)
#define __SELECTION32_3(b0, b1, b2) __BOOL32_REDEF3(b0, b1, b2)
#define __SELECTION32_4(b0, b1, b2, b3) __BOOL32_REDEF4(b0, b1, b2, b3)
#define __SELECTION32_5(b0, b1, b2, b3, b4) __BOOL32_REDEF5(b0, b1, b2, b3, b4)
#define __SELECTION32_6(b0, b1, b2, b3, b4, b5) __BOOL32_REDEF6(b0, b1, b2, b3, b4, b5)
#define __SELECTION32_7(b0, b1, b2, b3, b4, b5, b6) __BOOL32_REDEF7(b0, b1, b2, b3, b4, b5, b6)
#define __SELECTION32_8(b0, b1, b2, b3, b4, b5, b6, b7) __BOOL32_REDEF8(b0, b1, b2, b3, b4, b5, b6, b7)
#define __SELECTION32_9(b0, b1, b2, b3, b4, b5, b6, b7, b8) __BOOL32_REDEF9(b0, b1, b2, b3, b4, b5, b6, b7, b8)
#define __SELECTION32_10(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9) __BOOL32_REDEF10(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9)
#define __SELECTION32_11(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10) __BOOL32_REDEF11(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10)
#define __SELECTION32_12(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11) __BOOL32_REDEF12(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11)
#define __SELECTION32_13(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12) __BOOL32_REDEF13(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12)
#define __SELECTION32_14(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13) __BOOL32_REDEF14(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13)
#define __SELECTION32_15(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14) __BOOL32_REDEF15(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14)
#define __SELECTION32_16(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15) __BOOL32_REDEF16(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15)
#define __SELECTION32_17(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16) __BOOL32_REDEF17(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16)
#define __SELECTION32_18(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17) __BOOL32_REDEF18(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17)
#define __SELECTION32_19(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18) __BOOL32_REDEF19(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18)
#define __SELECTION32_20(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19) __BOOL32_REDEF20(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19)
#define __SELECTION32_21(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20) __BOOL32_REDEF21(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20)
#define __SELECTION32_22(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21) __BOOL32_REDEF22(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21)
#define __SELECTION32_23(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22) __BOOL32_REDEF23(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22)
#define __SELECTION32_24(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23) __BOOL32_REDEF24(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23)
#define __SELECTION32_25(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24) __BOOL32_REDEF25(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24)
#define __SELECTION32_26(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25) __BOOL32_REDEF26(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25)
#define __SELECTION32_27(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26) __BOOL32_REDEF27(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26)
#define __SELECTION32_28(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27) __BOOL32_REDEF28(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27)
#define __SELECTION32_29(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28) __BOOL32_REDEF29(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28)
#define __SELECTION32_30(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29) __BOOL32_REDEF30(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29)
#define __SELECTION32_31(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30) __BOOL32_REDEF31(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30)
#define __SELECTION32_32(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30, b31) __BOOL32_REDEF32(b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30, b31)

#define __SELECTION32_(N, ...) __SELECTION32_ ## N (__VA_ARGS__)
#define __SELECT32_(N, ...) __SELECTION32_(N, __VA_ARGS__)

#define BOOL32_REDEF(...) __SELECT32_(__NARGS(__VA_ARGS__), __VA_ARGS__)




/**********************************************************************
 *
 *  BOOL32_REDEF_PREF(...)
 *  Description:
 *  Rename b0...b31 with prefix of your choice
 *
 *  Usage:
 *  BOOL32_REDEF_PREF([prefix], [NAME0],...,[NAME31]) [varName]
 *  varName.prefixNAME0=[true/false]
 *  ...
 *  varName.prefixNAME31=[true/false]
 *
 *  varName( [index] ) - return bit according to index
 *  varname[ [index] ] - equivalent to varName([index])
 *  varName([index], [true/false]) - set state of
 *  bit according to index
 *  varName.getValue(void) - returns uint16_t representation
 *  of bits in BOOL32
 *  varName.size(void) - return constant count of bits
 *  varName = [unsigned int] - overloaded operator will make easier
 *  to copy data from unsigned int
 *
 *  NOTE: macro is overloaded, you do not need
 *  to rename all 32 bits
 *
 **********************************************************************/

#define __BOOL32_REDEF_PREF0(_prefix) __BOOL32_REDEF_PREF32(_prefix,0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF1(_prefix,b0) __BOOL32_REDEF_PREF32(_prefix,b0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF2(_prefix,b0,b1) __BOOL32_REDEF_PREF32(_prefix,b0,b1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF3(_prefix,b0,b1,b2) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF4(_prefix,b0,b1,b2,b3) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF5(_prefix,b0,b1,b2,b3,b4) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF6(_prefix,b0,b1,b2,b3,b4,b5) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF7(_prefix,b0,b1,b2,b3,b4,b5,b6) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF8(_prefix,b0,b1,b2,b3,b4,b5,b6,b7) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF9(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF10(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF11(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF12(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF13(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF14(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF15(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF16(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF17(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF18(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF19(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF20(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF21(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF22(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF23(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF24(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF25(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF26(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF27(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,27,28,29,30,31)
#define __BOOL32_REDEF_PREF28(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,28,29,30,31)
#define __BOOL32_REDEF_PREF29(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,29,30,31)
#define __BOOL32_REDEF_PREF30(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,30,31)
#define __BOOL32_REDEF_PREF31(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,b30) __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,30b,31)


#define __BOOL32_REDEF_PREF32(_prefix,b0,b1,b2,b3,b4,b5,b6,b7,b8,b9,b10,b11,b12,b13,b14,b15,b16,b17,b18,b19,b20,b21,b22,b23,b24,b25,b26,b27,b28,b29,b30,b31) \
    struct{\
        union{\
            struct{\
            uint8_t _prefix##b0 :1;\
            uint8_t _prefix##b1 :1;\
            uint8_t _prefix##b2 :1;\
            uint8_t _prefix##b3 :1;\
            uint8_t _prefix##b4 :1;\
            uint8_t _prefix##b5 :1;\
            uint8_t _prefix##b6 :1;\
            uint8_t _prefix##b7 :1;\
            uint8_t _prefix##b8 :1;\
            uint8_t _prefix##b9 :1;\
            uint8_t _prefix##b10 :1;\
            uint8_t _prefix##b11 :1;\
            uint8_t _prefix##b12 :1;\
            uint8_t _prefix##b13 :1;\
            uint8_t _prefix##b14 :1;\
            uint8_t _prefix##b15 :1;\
            uint8_t _prefix##b16 :1;\
            uint8_t _prefix##b17 :1;\
            uint8_t _prefix##b18 :1;\
            uint8_t _prefix##b19 :1;\
            uint8_t _prefix##b20 :1;\
            uint8_t _prefix##b21 :1;\
            uint8_t _prefix##b22 :1;\
            uint8_t _prefix##b23 :1;\
            uint8_t _prefix##b24 :1;\
            uint8_t _prefix##b25 :1;\
            uint8_t _prefix##b26 :1;\
            uint8_t _prefix##b27 :1;\
            uint8_t _prefix##b28 :1;\
            uint8_t _prefix##b29 :1;\
            uint8_t _prefix##b30 :1;\
            uint8_t _prefix##b31 :1;\
            };\
            uint32_t value;\
        };\
            /**Dynamic Write*/\
            void operator ()(const uint8_t index, const bool bState){\
                value = (value & ( ~(0x01 << index))) | (bState << index);\
            }\
    \
            /**Dynamic Read*/\
            inline bool operator () (const uint8_t index) const{\
                return (*this)[index];\
            }\
            /**Dynamic Read*/\
            bool operator [] (const uint8_t index) const{\
                return ((value >> index) & 0x01);\
            }\
    \
            inline uint32_t operator = (const uint32_t val){return value = val;}\
            inline uint32_t operator = (const BOOL32 b32){return value = b32.value;}\
            \
            inline uint32_t operator + (const uint32_t val){return (value + val);}\
	    inline uint32_t operator + (const BOOL32 b32){return (value + b32.value);}\
	    inline uint32_t operator - (const uint32_t val){return (value - val);}\
	    inline uint32_t operator - (const BOOL32 b32){return (value - b32.value);}\
	    inline uint32_t operator * (const uint32_t val){return (value * val);}\
	    inline uint32_t operator * (const BOOL32 b32){return (value * b32.value);}\
	    inline uint32_t operator / (const uint32_t val){return (value / val);}\
	    inline uint32_t operator / (const BOOL32 b32){return (value / b32.value);}\
	    \
	    inline uint32_t operator += (const uint32_t val){return (value += val);}\
	    inline uint32_t operator += (const BOOL32 b32){return (value += b32.value);}\
	    inline uint32_t operator -= (const uint32_t val){return (value -= val);}\
	    inline uint32_t operator -= (const BOOL32 b32){return (value -= b32.value);}\
	    inline uint32_t operator *= (const uint32_t val){return (value *= val);}\
	    inline uint32_t operator *= (const BOOL32 b32){return (value *= b32.value);}\
	    inline uint32_t operator /= (const uint32_t val){return (value /= val);}\
	    inline uint32_t operator /= (const BOOL32 b32){return (value /= b32.value);}\
    	    \
            inline uint32_t operator | (const uint32_t val){return (value | val);}\
	    inline uint32_t operator | (const BOOL32 b32){return (value | b32.value);}\
	    inline uint32_t operator & (const uint32_t val){return (value & val);}\
	    inline uint32_t operator & (const BOOL32 b32){return (value & b32.value);}\
	    inline uint32_t operator ^ (const uint32_t val){return (value ^ val);}\
	    inline uint32_t operator ^ (const BOOL32 b32){return (value ^ b32.value);}\
	    inline uint32_t operator << (const uint32_t val){return (value << val);}\
	    inline uint32_t operator >> (const uint32_t val){return (value >> val);}\
	    inline uint32_t operator ~ (void){return (~value);}\
	    \
	    inline uint32_t operator |= (const uint32_t val){return (value |= val);}\
	    inline uint32_t operator |= (const BOOL32 b32){return (value |= b32.value);}\
	    inline uint32_t operator &= (const uint32_t val){return (value &= val);}\
	    inline uint32_t operator &= (const BOOL32 b32){return (value &= b32.value);}\
	    inline uint32_t operator ^= (const uint32_t val){return (value ^= val);}\
	    inline uint32_t operator ^= (const BOOL32 b32){return (value ^= b32.value);}\
	    \
            inline const uint8_t size() const{return BOOL32_BIT_SIZE;}\
            \
            \
    }




#define __SELECTION_PREF32_1(_prefix) __BOOL32_REDEF_PREF0(_prefix)
#define __SELECTION_PREF32_2(_prefix,b0) __BOOL32_REDEF_PREF1(_prefix,b0)
#define __SELECTION_PREF32_3(_prefix,b0, b1) __BOOL32_REDEF_PREF2(_prefix,b0, b1)
#define __SELECTION_PREF32_4(_prefix,b0, b1, b2) __BOOL32_REDEF_PREF3(_prefix,b0, b1, b2)
#define __SELECTION_PREF32_5(_prefix,b0, b1, b2, b3) __BOOL32_REDEF_PREF4(_prefix,b0, b1, b2, b3)
#define __SELECTION_PREF32_6(_prefix,b0, b1, b2, b3, b4) __BOOL32_REDEF_PREF5(_prefix,b0, b1, b2, b3, b4)
#define __SELECTION_PREF32_7(_prefix,b0, b1, b2, b3, b4, b5) __BOOL32_REDEF_PREF6(_prefix,b0, b1, b2, b3, b4, b5)
#define __SELECTION_PREF32_8(_prefix,b0, b1, b2, b3, b4, b5, b6) __BOOL32_REDEF_PREF7(_prefix,b0, b1, b2, b3, b4, b5, b6)
#define __SELECTION_PREF32_9(_prefix,b0, b1, b2, b3, b4, b5, b6, b7) __BOOL32_REDEF_PREF8(_prefix,b0, b1, b2, b3, b4, b5, b6, b7)
#define __SELECTION_PREF32_10(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8) __BOOL32_REDEF_PREF9(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8)
#define __SELECTION_PREF32_11(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9) __BOOL32_REDEF_PREF10(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9)
#define __SELECTION_PREF32_12(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10) __BOOL32_REDEF_PREF11(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10)
#define __SELECTION_PREF32_13(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11) __BOOL32_REDEF_PREF12(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11)
#define __SELECTION_PREF32_14(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12) __BOOL32_REDEF_PREF13(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12)
#define __SELECTION_PREF32_15(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13) __BOOL32_REDEF_PREF14(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13)
#define __SELECTION_PREF32_16(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14) __BOOL32_REDEF_PREF15(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14)
#define __SELECTION_PREF32_17(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15) __BOOL32_REDEF_PREF16(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15)
#define __SELECTION_PREF32_18(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16) __BOOL32_REDEF_PREF17(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16)
#define __SELECTION_PREF32_19(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17) __BOOL32_REDEF_PREF18(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17)
#define __SELECTION_PREF32_20(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18) __BOOL32_REDEF_PREF19(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18)
#define __SELECTION_PREF32_21(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19) __BOOL32_REDEF_PREF20(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19)
#define __SELECTION_PREF32_22(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20) __BOOL32_REDEF_PREF21(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20)
#define __SELECTION_PREF32_23(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21) __BOOL32_REDEF_PREF22(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21)
#define __SELECTION_PREF32_24(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22) __BOOL32_REDEF_PREF23(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22)
#define __SELECTION_PREF32_25(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23) __BOOL32_REDEF_PREF24(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23)
#define __SELECTION_PREF32_26(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24) __BOOL32_REDEF_PREF25(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24)
#define __SELECTION_PREF32_27(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25) __BOOL32_REDEF_PREF26(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25)
#define __SELECTION_PREF32_28(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26) __BOOL32_REDEF_PREF27(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26)
#define __SELECTION_PREF32_29(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27) __BOOL32_REDEF_PREF28(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27)
#define __SELECTION_PREF32_30(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28) __BOOL32_REDEF_PREF29(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28)
#define __SELECTION_PREF32_31(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29) __BOOL32_REDEF_PREF30(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29)
#define __SELECTION_PREF32_32(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30) __BOOL32_REDEF_PREF31(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30)
#define __SELECTION_PREF32_33(_prefix,b0, b1, b2, b3, b4, b5, b6, b7,b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30, b31) __BOOL32_REDEF_PREF32(_prefix,b0, b1, b2, b3, b4, b5, b6, b7, b8, b9, b10, b11, b12, b13, b14, b15, b16, b17, b18, b19, b20, b21, b22, b23, b24, b25, b26, b27, b28, b29, b30, b31)

#define __SELECTION_PREF32(N, ...) __SELECTION_PREF32_ ## N (__VA_ARGS__)
#define SELECT_PREF32(N, ...) __SELECTION_PREF32(N, __VA_ARGS__)

#define BOOL32_REDEF_PREFIX(...) SELECT_PREF32(__NARGS(__VA_ARGS__), __VA_ARGS__)




#endif // BITBOOL_H

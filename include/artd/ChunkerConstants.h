/**
 * @author Peter Kennard
 * 
 * This code is an implementation of the "Binary Markup Language"
 * of which similar implementations have been provided to other 
 * non-exclusive clients of the aRt&D Lab.  
 * 
 * The algorithms defining the encoding and decoding of the 
 * "Binary Markup Language"
 * Copyright (c) 1998-2013 Peter Kennard and aRt&D Lab
 *  
 *  With respect to the "Binary Markup Language" and it's 
 *  defining algorithms:
 *  
 * 1. Redistributions of the source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *  
 * THE PORTIONS OF THIS SOFTWARE DERIVED FORM THE "Binary Markup Languange"
 * IS PROVIDED ``AS IS'', WITHOUT ANY WARRANTIES, EXPRESS
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  LICENSOR SHALL
 * NOT BE LIABLE FOR ANY LOSS OR DAMAGES RESULTING FROM THE USE OF THIS
 * SOFTWARE, EITHER ALONE OR IN COMBINATION WITH ANY OTHER SOFTWARE.
 */


#ifndef __artd_ChunkerConstants_h
#define __artd_ChunkerConstants_h

#include "artd/jlib_io.h"
#include "artd/pointer_math.h"
#include "artd/int_types.h"
#include "artd/IdMap.h"

ARTD_BEGIN

class TextWriter;

class ChunkerConstants
{

public:

	typedef uint32_t    length_t;

	static const int MaxClassIdLength = 32; // in bytes

	static ARTD_API_JLIB_IO const char nullString[]; // "null"

	/**
	* special node Id to indicate an "untagged" node
	*/
	static const int UnTaggedId = -1;
	static ARTD_API_JLIB_IO const char UnTaggedStringId[]; // ""

	// static const IdMap indexedIdMap_ = new IdMap() {
	// @Override
	// public String get(int id) {
	// if(id == UnTaggedId) {
	// return(UnTaggedStringId);
	// }
	// return(Long.toString(0x0FFFFFFFF & (long)id));
	// }
	// @Override
	// public int get(String name) {
	// return((int)ParseTools.fastParseUnsigned(name));
	// }
	// };

	/** definition of ChunkHead bits embedded in the length word
	* The incoming bits. The head format is based on a "VUint" type
	* or Variable length unsigned integer like UTF 8 but where the high bit 7
	* indicates a subsequent byte of lower order bits is next
	*
	* The head is composed of VUints the first VUint is a
	* "length|type for a chunk or node"
	*
	* |n ... 3|  - length of data in bytes after this VUint
	*         |2..0| - 3 "high" bits of type the "LType"
	*
	*  for nodes with a type and ID field in the header the header
	*  will be only two bytes for ids of 0 to 0x7F and data lengths of 0 to 0xF
	*  all numeric primitives to a 15 bytes will have an added two byte
	*  for length,type, and id each subsequent 7 bits of length or id will add a byte of header
	* */

	/** number of bits to shift over for length value */
	static const int LengthValueShift = 3;

	/** mask to get upper 3 bits of type (8 values)
	* if NodeHasCompositeId flag is ON
	*/
	static const int LTypeMask = 0x07;

	/** If Length Type (LType) is < "LTypeNode" the following word is a composite
	* of an Id and Type value (unsigned integers)
	* An IdType has the lower 3 bits of subtype
	* giving 16 total "primitive" types from  0 to 15
	*/

	/** bits of type stored in id word */
	static const int IdTypeMask = 0x03;
	/** number of bits to shift over to get ID value incomposite ID/Type int */
	static const int IdTypeShift = 2;

	/** 16 types expressed by the composite length/type id/type combo */

	/* LType == 0 single variable length data primitive nodes
	* 0 + 0..3
	* Since no LType == 0 primitive type is valid with a length of zero,
	* a single byte of 0 is used to represent a no-op. or pad byte.
	*
	* Zeros therefore can be used as filler between
	* chunks for alignment and/or to rewrite length/type
	* words embedded in previously formatted and aligned buffers.
	*/
	static const int ChunkByteArray = 0;
	/** Chunk contains a signed integer of node length
	* with high bit (last bit) interpreted as a sign bit
	*/
	static const int ChunkInt = 1;
	/** Chunk contains an unsigned integer of node length
	* with high bit *NOT* interpreted as a sign bit
	*/
	static const int ChunkUint = 2;
	/** Chunk contains real (float) of node length
	* encoded in variable real number format
	*/
	static const int ChunkReal = 3;

	/** LType1 types from 4 to 7 fixed length array structure is:
	* 4 + 0..3
	* length:id:type,elementSize,[elements]
	*/
	static const int ChunkCharArray = 4;
	static const int ChunkIntArray = 5;
	static const int ChunkUIntArray = 6;
	static const int ChunkRealArray = 7;

	/** LType2 types 8 to 10 variable length element array structure is<br>
	* length:id:type,elementCount,[elements]
	*/
	static const int ChunkVCharArray = 8;
	static const int ChunkVIntArray = 9;
	static const int ChunkVUIntArray = 10;
	// static const int ChunkVRealArray = 11; // so far this isn't used anywhere

	/* alias for string */
	static const int ChunkString = ChunkVCharArray;

	// LType3 ...
	static const int ChunkBoolean = 12; /// length:id:type (length always 0 (false only) or 1 (true only))
	static const int ChunkClassId = 13; /// length:id:type:classIdData
	/**
	* Node containing an object of "classId" type a length of 0 is NULL
	* the classId is a length + byte array len = Vint0, classId[len] = { byte1, ... byteLen-1 }
	*/
	static const int ChunkExtendedTypeInt = 14; /// length:id:type:extendedType[contents]
	static const int ChunkDelimiter = 15;   /// lengthOfThisNode:startType:type[contents]
	/**
	* ChunkNodeBegin and ChuinkListBegin are
	* delimited chunks without byte count - have a begin chunk and are terminated by a ChunkDelimiter delimiter
	* a "list" can have duplicate IDs within it in a Node child Id's are assumed to be unique
	* and if not later chunks with the same ID will "overwrite" prior chunks with the same ID
	* the "slassId" is optional and only present if lengthToNode is greater that the length of the id
	* This is similar to the difference between attributes (Nodes) and tagged blocks (Lists) in XML or
	* Objects and Arrays in Json.
	*/
	/**
	* Identified parent Nodes with or without length and a potentially known number of Children<br>
	* The Block items contain the length in bytes of the contained block, and may contain the count of the
	* contained elements.  They are delimited by the length in bytes in a BML file.
	*/
	// LType4 = 4;

	// items below here contain classIds
	static const int ChunkNodeBegin = 16;   /// lengthToNode:id:type[classId] { children } ChunkDelimiter
	static const int ChunkListBegin = 17;   /// lengthToList:id:type[classId] [ list of children all of classType if present ] ChunkDelimiter
											// if length == 0 this is a NULL
	static const int ChunkObjectNode = 18;  /// length:id:type:classIdLen,classId[classIdLen]:contents
	static const int ChunkListNode = 19;  /// length:id:type:clsssIdLen,classId[classIdLen]:contents   [ list(array) of children unknown count ]

	/**
	* Identified untyped (no classId) Node with unknown number of Children<br>
	* and delimited by a known byte length
	* length:id { children }
	* This used for child objects when the type of the object expected is know because the
	* Parent node has specified what type of object it is expecting when loading it's fields.
	* If the parent node dies not specift or specifies "Object" it will be loaded as a geenric mapped object.
	* also used for minimal length "opcodes" where all you want is a compact ID.
	*/
	static const int LTypeNode = 5;
	static const int ChunkNode = 20;

	/* anonymous Types */

	/**
	* Anonymous Node with
	* contained children and known length<br>
	* No children if length == 0<br>
	* structure is:
	* length { children }
	* And UnTagged Node with a length of 0 is an anonymous "NULL" Object
	*/
	static const int LTypeUntaggedNode = 6;
	static const int ChunkUntaggedNode = 21;

	/**
	* Anonymous byte array "payload" of known length.
	* with no subtype and no ID.
	*/
	static const int LTypePayload = 7;
	static const int ChunkPayload = 22;

	static const int ChunkMaxType = ChunkPayload;

	class ARTD_API_JLIB_IO ToolsClass {

		friend class ChunkerConstants;

	public:
		static const int fPrimitiveType = 0x01;
		static const int fNumberType = 0x02;
        static const int fArrayType  = 0x04;
        static const int fVArrayType = 0x08;

		static const int fContainerType = 0x10;
		static const int fListType = 0x20;
	private:

		const char *typeStrings_[0x020];
		// NOT YET const unsigned char *typeClassIds_[0x020];
		int tFlags_[0x020];
	
	protected:
		~ToolsClass() {}
		ToolsClass();

	public:

		ARTD_ALWAYS_INLINE static bool isNumberType(int type) {
			return(type >= ChunkInt && type <= ChunkReal);
		}
		ARTD_ALWAYS_INLINE static bool isIntegerType(int type) {
			return(type >= ChunkInt && type <= ChunkUint);
		}
        ARTD_ALWAYS_INLINE bool isArrayType(int type) const {
            return((tFlags_[type & 0x01f] & fArrayType) != 0);
        }
        ARTD_ALWAYS_INLINE bool isVArrayType(int type) const {
            return((tFlags_[type & 0x01f] & fVArrayType) != 0);
        }
		ARTD_ALWAYS_INLINE bool isListType(int type) const {
			return((tFlags_[type & 0x01f] & fListType) != 0);
		}
		ARTD_ALWAYS_INLINE bool isObjectType(int type) const {
			return((tFlags_[type & 0x01f] & fContainerType) != 0);
		}
		ARTD_ALWAYS_INLINE bool hasChildren(int type) const {
			return((tFlags_[type & 0x01f] & (fListType | fContainerType)) != 0);
		}

		//byte[] getPrimtiveTypeClassId(int type) const {
		//	if ((tFlags_[type] & fPrimitiveType) != 0) {
		//		return(typeClassIds_[type & 0x01f]);
		//	}
		//	return(null);
		//}

		static void appendIndent(TextWriter* w, int indent, int level);

		ARTD_ALWAYS_INLINE const char*getTypeName(int chunkType) const {
			if ((chunkType & 0x0FF) < ARTD_ARRAY_SIZE(typeStrings_)) {
				return(typeStrings_[chunkType]);
			}
			return("Unknown"); //  +chunkType);
		}
	}; // end ToolsClass

public:

	static ARTD_API_JLIB_IO const ToolsClass Tools;

};

ARTD_END

#endif // __artd_ChunkerConstants_h

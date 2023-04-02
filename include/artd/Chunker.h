#ifndef __artd_Chunker_h
#define __artd_Chunker_h

/**
 * @author Peter Kennard
 *
 * This code is an implementation of the "Binary Markup Language"
 * Binary Data Ecapsulation "Chunker"
 * of which similar implementations have been provided to other
 * non-exclusive clients of the aRt&D Lab.
 *
 * The algorithms defining the encoding and decoding of the
 * "Binary Markup Language"
 * Copyright (c) 1998-2017 Peter Kennard and aRt&D Lab
 *
 *  With respect to the "Binary Markup Format" and it's
 *  defining algorithms:
 *
 * 1. Redistributions of the source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * THE PORTIONS OF THIS SOFTWARE DERIVED FORM THE "Binary Markup Format"
 * IS PROVIDED ``AS IS'', WITHOUT ANY WARRANTIES, EXPRESS
 * OR IMPLIED, INCLUDING, BUT NOT LIMITED TO, WARRANTIES OF
 * MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.  LICENSOR SHALL
 * NOT BE LIABLE FOR ANY LOSS OR DAMAGES RESULTING FROM THE USE OF THIS
 * SOFTWARE, EITHER ALONE OR IN COMBINATION WITH ANY OTHER SOFTWARE.
 *
 */
 
#include "artd/ObjectBase.h"
#include "artd/ChunkerConstants.h"
// #include "artd/ArtdClassId.h"
#include "artd/StringArg.h"

ARTD_BEGIN

class ArtdClassId;

#ifdef __EMSCRIPTEN__
	#pragma GCC diagnostic ignored "-Wkeyword-macro"
#endif
#define INL ARTD_ALWAYS_INLINE

// note in MSC export forces INL methods to be linked !!!!
class /* ARTD_API_JLIB_IO */ Chunker
	: public ObjectBase
	, public ChunkerConstants
{
public:

	typedef StringArg String;

    virtual int flush() = 0;
    virtual int close() = 0;

    /**
     * reset stack back to root level
     */
    virtual void reset() = 0;

    virtual int pushUnTaggedNode() = 0;

    /**
     * Begin a tagged chunk "node" of unknown length
     * @param id - the node's id tag
     * @throws IOException
     */
    virtual int pushNode(int id) = 0;
//    virtual void pushNode(String id) = 0;


    /**
     * Begin a chunk "ObjectNode" of unknown length
     * @param id - the node's id tag
     * @param type - classId to include in the node
     * @throws IOException
     */
    virtual int pushTypedNode(int id, const ArtdClassId&type) = 0;
//    virtual int pushTypedNode(String id, const ArtdClassId &type) = 0;

    /**
     * Begin a tagged chunk "list" (tag delimited) of unknown length
     * @param id - the list's id tag
     * @throws IOException
     */
    virtual int pushList(int id) = 0;
 //   virtual int pushList(String id) = 0;

    /**
     * Begin a tagged chunk "list" (tag delimited) of unknown length
     * @param id - the list's id tag
     * @param type - classId of list elements to include in the node
     * @throws IOException
     */
    virtual int pushTypedList(int id, const ArtdClassId &type) = 0;
//    virtual void pushTypedList(String id, const ArtdClassId &type) = 0;

    /**
     * Sets the "IdMap" for the current level, if the IdMap for a level is not set it
     * will inherit the IdMap from the parent (prior) level when the node is pushed.
     * If the current level is the root level the root level IdMap is set.
     * If the current level is "popped" the setting for the IdMap is popped as well
     * restoring the parent level's IdMap as the current setting.
     * @param map The IdMap to set for the current level.
     */
	virtual void setIdMap(IdMap *map) = 0;

	/**
	* gets the IdMap for the current level
	* @return the current level's id map.
	*/
	virtual IdMap *getIdMap() = 0;

    /**
     * @return current chunk recursion level 0 == root
     * -1 == below root level, buffer empty.
     */
    virtual int level() = 0;

    /**
     * Terminate current node, writing length and flushing
     * contents from internal buffers for output stream.
     * @return 	level popped to < 0 if popped root level
     * @throws IOException
     */
    virtual int popNode() = 0;

    /**
     * Get PrimWriter for writing contents of current Node
     * @return
     *     The writer
     */
    // virtual WriterT getWriter() = 0;

    /**
     * When output is formatted text (Json, XML, etc) sets "pretty" flag indicating
     * output is to be indented, and contain line breaks and white space between elements.
     * to improve legibility.  Default is off (false)
     * @param on
     */
    virtual void setPretty(int indentLevel) = 0;

    /**
     * return "pretty" indent level <= 0 == off
     * @return
     */
    virtual int getPretty() = 0;

    /**
     * set debug logging on/off
     * @param on
     */
    virtual void setDebug(bool on) = 0;

    /**
     * Write an Empty ChunkNode node to the output stream.
     * @param id - Id of node
	 * @return number of bytes added to output < 0 if error
	 */
    virtual int writeEmptyNode(int id) = 0;
//    virtual int writeEmptyNode(String id) = 0;

    /**
     * Write a String to the output stream as VIntU chars<br>
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id - Id of node
     * @param s - the string to write
	 * @return number of bytes added to output < 0 if error
	 */
	virtual int writeStringNode(int id, StringArg s) = 0;
	virtual int writeStringNode(int id, WStringArg s) = 0;

	//    virtual int writeStringNode(String id, String s) = 0;

    /**
     * Write an single integer node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write
     * @param value value for node
	 * @return number of bytes added to output < 0 if error
	 */
    virtual int writeIntNode(int id, long value) = 0;
//    virtual int writeIntNode(String id, long value) = 0;


	/**
	* Write an single unsigned integer node
	* Must only be called inside a node on level 0 or above
	* will not work to write a root node.
	* @param id Id of node to write may not be "UnTaggedId"
	* @param value value for node
	* @return number of bytes added to output < 0 if error
	*/
	virtual int writeIntUNode(int id, uint64_t value) = 0;
	// virtual int writeIntUNode(String id, uint64_t value) = 0;


	/**
	* Write an single unsigned integer node
	* Must only be called inside a node on level 0 or above
	* will not work to write a root node.
	* @param id Id of node to write may not be "UnTaggedId"
	* @param value value for node
	* @return number of bytes added to output < 0 if error
	*/
	INL int writeIntUNodeRef(int id, const unsigned int &value) {
		return(writeIntUNode(id, ((uint64_t)value) & 0x0FFFFFFFF));
	}
	INL int writeIntUNode(int id, const int &value) {
		return(writeIntUNode(id, ((uint64_t)value) & 0x0FFFFFFFF));
	}


	/**
	* Write an single unsigned integer node
	* Must only be called inside a node on level 0 or above
	* will not work to write a root node.
	* @param id Id of node to write may not be "UnTaggedId"
	* @param value value for node
	* @return number of bytes added to output < 0 if error
	*/
	INL int writeIntUNode(int id, const int8_t &value) {
		return(writeIntUNode(id, (uint64_t)((int)value) & 0xFF));
	}
	INL int writeIntUNode(int id, const uint8_t &value) {
		return(writeIntUNode(id, (uint64_t)((int)value) & 0xFF));
	}
	/**
	* Write an single unsigned integer node
	* Must only be called inside a node on level 0 or above
	* will not work to write a root node.
	* @param id Id of node to write may not be "UnTaggedId"
	* @param value value for node
	* @return number of bytes added to output < 0 if error
	*/
	INL int writeIntUNode(int id, const short &value) {
		return(writeIntUNode(id, (uint64_t)((int)value) & 0xFFFF));
	}
	INL int writeIntUNode(int id, const unsigned short &value) {
		return(writeIntUNode(id, (uint64_t)((int)value) & 0xFFFF));
	}

    /**
     * Write an single boolean node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write
     * @param value value for node
	 * @return number of bytes added to output < 0 if error
     */
    virtual int writeBooleanNode(int id, bool value) = 0;
//    virtual int writeBooleanNode(String id, bool value) = 0;

    /**
     * Write an single float (real number) node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write may not be "UnTaggedId"
     * @param value value for node
	 * @return number of bytes added to output < 0 if error
	 */
    virtual int writeFloatNode(int id, double value) = 0;
//    virtual int writeFloatNode(String id, double value) = 0;

    /**
     * Write ByteArray Node
     * @param id Id of node to write
     * @param val value for node
     * @param offset of data
     * @param length length of data
	 * @return number of bytes added to output < 0 if error
	 */
	virtual int writeByteArrayNode(int id, const void *val, int length) = 0;
	//    virtual int writeByteArrayNode(String id, const void *val, int length) = 0;

    /**
     * Write ByteArray Node.
     * @param id Id of node to write
     * @param val value for node
	 * @return number of bytes added to output < 0 if error
	 */
    //virtual int writeByteArrayNode(int id, int8_t val[]) = 0;
    //virtual int writeByteArrayNode(String id, int8_t val[]) = 0;

    /**
     * Write IntArray Node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write
     * @param val value for node
     * @param offset index of data
     * @param length element length of data
	 * @return number of bytes added to output < 0 if error
	 */

	virtual int writeIntArrayNode(int id, const int16_t *val, int length) = 0;
	//   virtual int writeIntArrayNode(String id, int16_t val[], int offset, int length) = 0;
    /**
     * Write IntArray Node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write
     * @param val value for node
     * @param offset index of data
     * @param length element length of data
	 * @return number of bytes added to output < 0 if error
	 */
	virtual int writeIntArrayNode(int id, const int32_t *val, int length) = 0;
	//    virtual int writeIntArrayNode(String id, int32_t val[], int offset, int length) = 0;

    /**
     * Write IntArray Node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write
     * @param val value for node
     * @param offset index of data
     * @param length element length of data
	 * @return number of bytes added to output < 0 if error
	 */
    virtual int writeIntArrayNode(int id, const int64_t *val, int length) = 0;
//    virtual int writeIntArrayNode(String id, long val[], int offset, int length) = 0;

    /**
     * Write IntArray Node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write
     * @param val value for node
     * @param offset index of data
     * @param length element length of data
	 * @return number of bytes added to output < 0 if error
	 */
	virtual int writeIntUArrayNode(int id, const uint16_t *val, int length) = 0;
    virtual int writeIntUArrayNode(int id, const uint32_t *val, int length) = 0;
    //    virtual int writeIntUArrayNode(String id, int val[], int offset, int length) = 0;

    /**
     * Write IntArray Node
     * Must only be called inside a node on level 0 or above
     * will not work to write a root node.
     * @param id Id of node to write
     * @param val value for node
     * @param offset index of data
     * @param length element length of data
	 * @return number of bytes added to output < 0 if error
	 */
    virtual int writeIntUArrayNode(int id, const uint64_t *val, int length) = 0;
//    virtual int writeIntUArrayNode(String id, long val[], int offset, int length) = 0;

    /////////////// *************** Object "stream" methods
    // TODO: maybe this should be in an object streamer ?

    /**
     * @param id Id of node to write
     * @param obj value for node
	 * @return number of bytes added to output < 0 if error
	 */
//    virtual int writeObject(int id, Object obj) = 0;
//    virtual int writeObject(String id, Object obj) = 0;

    /**
     * @param id Id of node to write
     * @param obj value for node
     * @param artdClass  The class of the object - MUST be tha actual class of the object.
	 * @return number of bytes added to output < 0 if error
	 */
    // virtual int writeObjectOfClass(int id, Object obj, ArtdClass<?> artdClass) = 0;
    // virtual int writeObjectOfClass(String id, Object obj, ArtdClass<?> artdClass) = 0;

    /**
     * write representation of "null" object to stream
     * @param id
	 * @return number of bytes added to output < 0 if error
     */
    virtual int writeNullObject(int id) = 0;
//    virtual int writeNullObject(String id) = 0;

    /**
     * Write ObjectArray
     * @param val value for node
     * @param offset start index of array
     * @param length length of array
     * @throws IOException
     */
    // virtual void writeObjectArray(Object[] val, int offset, int length) = 0;
    // virtual void writeObjectArray(int id, Object[] val) = 0;
    // virtual void writeObjectArray(String id, Object[] val) = 0;
    // virtual void writeObjectArray(int id, Object[] val, int offset, int length) = 0;
    // virtual void writeObjectArray(String id, Object[] val, int offset, int length) = 0;
    // virtual void writeObjectArray(Collection<?> val) = 0;
    // virtual void writeObjectArray(int id, Collection<?> val) = 0;
    // virtual void writeObjectArray(String id, Collection<?> val) = 0;
};

#undef INL

ARTD_END

#endif // __artd_Chunker_h

#ifndef __artd_UnChunker_h
#define __artd_UnChunker_h

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
#include "artd/StringArg.h"
#include "artd/ArtdClassId.h"
#include "artd/TextWriter.h"
#include "artd/RcArray.h"

ARTD_BEGIN

class RcWString;
class PrimReader;

class ARTD_API_JLIB_IO UnChunker
	: public ObjectBase
	, public ChunkerConstants
{
public:
	typedef StringArg String;


	/** return code for nextNode() - an error has been encountered
	*/
	static const int NtError = -1;
	/** return code for nextNode() - end of current level (popLevel() to parent)
	*/
	static const int NtEnd = 0;
	/** return code for nextNode() -
	*  read node's length and positioned stream to the start of node's data
	*/
	static const int NtNode = 1;

	/**
	* reset this UNChunker to the root level state
	* starting at byte 0, It does NOT seek in re reset the input stream.
	*/
	virtual void reset() = 0;

	/**
	* aborts next read with nextNode returning NtEnd
   	*/
	virtual void abort() = 0;
	
	/**
	 * It this UnChunker is in a error state and the next call to nextNode() will return NtError
	 */
	virtual bool inErrorState() = 0;

	/**
	*
	* @return Status Code for item Encountered<br>
	* NtError - an error has been encountered<br>
	* NtEnd - End of current level, it contains no more child nodes<br>
	* NtNode - A node on the currnet level has been encountered.
	*/
	virtual int nextNode() = 0;

	/**
	* push a level down on the parse stack.
	* The next call to nextNode() will position it to the
	* first child on this level.
	*/
	virtual void pushLevel() = 0;

	/**
	* Sets the "IdMap" for the current level, if the IdMap for a level is not set it
	* will inherit the IdMap from the parent (prior) level.  If the current level is the root level
	* the root level IdMap is set.  If the current level is "popped" the setting for the IdMap is popped as well
	* restoring the parent level's IdMap as the current setting.
	* @param map The IdMap to set for the current level.
	*/
	virtual void setIdMap(IdMap *map) = 0;
	inline void setIdMap(IdMap &map) {
		setIdMap(&map);
	}

	/**
	* set debug trace printing on
	* @param on
	*/
	virtual void setDebug(bool on) = 0;
	/**
	* gets the IdMap for the current level
	* @return the current level's id map.
	*/
	virtual IdMap &getIdMap() = 0;

	/**
	* Convenience method that both pushes the current level and sets the IdMap
	* @param map  The IdMap to set for the pushed level
	*/
	virtual void pushLevel(IdMap *map) = 0;

	/**
	* @return current tree level for current node 0 == root level
	* < 0 if uninitialized to root level (in new or reset state)
	*/
	virtual int level() = 0;

	/**
	* pops back to parent from current level
	* after, nextNode() will position to next child on lower
	* level.
	* @return 0 if successful < 0 if not
	*/
	virtual int popLevel() = 0;

	/**
	* will return to a given level from the current one
	* good for "finally" clauses
	* @param level
	*/
	virtual void popBackToLevel(int level) = 0;

	/**
	* skips all child nodes on current level to the end of the current node
	* @return count of child nodes skipped < 0 if error
	* @throws IOException
	*/
	virtual int skipToEnd() = 0;

	/**
	* Read retrieve Id for current node.
	* @return  ID value for current node on current level untagged nodes return 0
	*/
	virtual int nodeId() = 0;

	/**
	* get the string name for the current node returns "" for an untagged node
	* @return String name for the current node
	*/
	virtual RcString nodeName() = 0;

	/**
	* @return Node type for current node
	*/
	virtual int nodeType() = 0;

	/**
	* @return classId of current node
	* if there is no classId itwill return null.
	* If it is a reserved or primtive classId it might return
	* An immutable comon ID
	* If not it willload toLoad with the data and return
	* it.
	* @param toLoad
	*/
	virtual ArtdClassId *nodeClassId(ArtdClassId &toLoad) = 0;

	/**
	* @return length of current node binary data if it is known.
    * < 0 if unknown.
	*/
	virtual int nodeLength() = 0;

    /**
     * Get the reader set up for the current node.
     * TODO: EOF will be reported when the entire node has been read. ( EOF not fully implemented !!! )
     * In the case of node types when the length is known it will
     * be when the reported length is exhausted.
     * When the length is not known it will be when the terminating delimiter is encountered.
     */
    virtual PrimReader *getNodeReader() = 0;

	/**
	* return if the current node can have children.
	* @return
	*/
	virtual bool nodeHasChildren() = 0;

	/**
	* return true if the node data has been read.
	* @return
	*/
	virtual bool nodeComplete() = 0;

	/**
	* @return  PrimReader for this stream for reading node data
	* Do Not use the stream from getInputStream in a node parser !!!
	*/
//	virtual ReaderT getNodeReader() = 0;

	/**
	* Read current leaf node as a String
	* @return the string read from the stream
	* @throws IOException
	*/
	virtual RcWString readStringNode() = 0;

	virtual bool readBooleanNode() = 0;

	virtual int64_t readIntNode() = 0;

	virtual uint64_t readIntUNode() = 0;

	virtual double readFloatNode() = 0;


	virtual ByteArray readByteArrayNode() = 0;


    /**
     * if the node is an array node it gets the length (number of array elements)
     * returns -1 if the node is not an array node or there is an error.
     */
    virtual int64_t   arrayNodeLength() = 0;

    /**
     * read array from current node into buffer
     * returns <= if error.
     * otherwise the number of elements read
     * the buffer must be large enough to fit the number of elements returned by
     * arrayNodeLength() above 
     */
    virtual int64_t readShortArrayNode(int16_t *buffer) = 0;
    virtual int64_t readIntArrayNode(int32_t *buffer) = 0;
    virtual int64_t readIntUArrayNode(uint32_t *buffer) = 0;
    virtual int64_t readIntArrayNode(int64_t *buffer) = 0;
    virtual int64_t readIntUArrayNode(uint64_t *buffer) = 0;

    virtual int64_t readFloatArrayNode(float *buffer) = 0;
    virtual int64_t readDoubleArrayNode(double *buffer) = 0;

	//virtual ObjectPtr<Object> readObjectFor(ArtdClass clas)
	//{
		//for (;;) {
		//	int type = nodeType();
		//	if (type < 0) {
		//		if (nodeId() == -1 && level() == 0) {
		//			if (nextNode() == NtNode) {
		//				continue;
		//			}
		//		}
		//	}
		//	ArtdClassId assignTo = clas.getClassId();
		//	ArtdClassId cid = nodeClassId(null);
		//	if (cid != null && !assignTo.equals(cid)) {
		//		ArtdClass actual = ClassManager.classForClassId(cid);
		//		if (actual == null) {
		//			throw new IOException("ClassID in stream not registered.");
		//		}
		//		else if (!clas.jClass_.isAssignableFrom(actual.jClass_)) {
		//			throw new ClassCastException("can't assign " + actual.jClass_.getCanonicalName() + " to " + clas.jClass_.getCanonicalName());
		//		}
		//		clas = actual;
		//	}
		//	return(clas.readObject(this));
		//}
	//	return(NULL);
	//}


	//virtual <T> T readObject(Class clas)
	//	throws IOException
	//{
	//	for (;;) {
	//		int type = nodeType();
	//		if (type < 0) {
	//			if (nodeId() == -1 && level() == 0) {
	//				if (nextNode() == NtNode) {
	//					continue;
	//				}
	//			}
	//		}
	//		if (Tools.isObjectType(type)) {
	//			@SuppressWarnings("unchecked")
	//			T ret = (T)ClassManager.classForClass(clas).readObject(this);
	//			return (ret);
	//		}
	//		else if (Tools.isListType(type)) {
	//			@SuppressWarnings("unchecked")
	//			T ret = (T)ClassManager.classForClass(clas).readObject(this);
	//			return(ret);
	//		}
	//		return (null);
	//	}
	//}

	//TransferPtr<Object> readObjectNode() {
	//	int type = nodeType();
	//	if (type < 0) {
	//		if (nodeId() == -1 && level() == 0) {
	//			nextNode();
	//		}
	//	}
	//	return(loadObjectFromChunk());
	//}

	//void dumpTree(Appendable out, int flags)
	//{
	//	int type = nodeType();
	//	if (type <= 0) {
	//		int id = nodeId();
	//		if (id == -1 && level() <= 0) {
	//			nextNode();
	//		}
	//	}
	//	int level = level();
	//	Tools.dumpTree(out, this, level);
	//}

	ObjectPtr<ObjectBase> loadObjectFromChunk()
	{
		//Object value = NULL;
		//switch (nodeType()) {
		//case ChunkReal:
		//	value = readFloatNode();
		//	break;
		//case ChunkString:
		//	value = readStringNode();
		//	break;
		//case ChunkInt:
		//	value = readIntNode();
		//	break;
		//case ChunkUint:
		//	value = readIntUNode();
		//	break;
		//case ChunkBoolean:
		//	value = readBooleanNode();
		//	break;
		//case ChunkExtendedTypeInt:  // JSON's null
		//	break;
		//case ChunkNodeBegin:
		//case ChunkListBegin: {
		//	//nodeClassId(null);
		//	break;
		//}
		//case ChunkNode:
		//case ChunkUntaggedNode:

		//	int oldLevel = level();
		//	try {
		//		pushLevel();
		//		int ret = nextNode();
		//		Class< ? > jClass = GenericMappedObject.class;
		//		ArtdClass< ? > aClass = null;
		//		if (ret == UnChunker.NtEnd) {
		//			value = new GenericMappedObject(); // empty node, return empty generic object !!
		//			break;
		//		}
		//		//                    else if(ret == NtNode && nodeType() == ChunkClassId) {
		//		//                        aClass = ClassManager.classForClassId(readClassIdNode());
		//		//                    }
		//		if (aClass == null) {
		//			aClass = ClassManager.classForClass(jClass);
		//		}
		//		value = aClass.instantiateAndLoad(this);
		//		break;
		//	}
		//	finally {
		//		popBackToLevel(oldLevel);
		//	}
		//case ChunkListNode:
		//	value = readObject(List.class);
		//	break;
		//}
		//return(value);

		return(nullptr);
	}


	class ARTD_API_JLIB_IO ToolsClass
		: public ChunkerConstants::ToolsClass
	{
		friend class UnChunker;

	protected:
		ToolsClass() {}

		//static void puts(Appendable out,Object s)
		//	throws IOException
		//{
		//	out.append(' ');
		//	out.append(s.toString());
		//	out.append('\n');
		//}

		///**
		//* to show the tree structure of a stream as unchunked (with or without primitive data types: unimplemented)
		//* @param out       appender to dump output to
		//* @param uc        UnChunker to dump from
		//* @param hideData  just print field names and types, not contents
		//* @throws IOException
		//*/

	public:
		~ToolsClass() {}
		static int dumpTree(TextWriter *out, UnChunker &uc, int level);
	};

public:

	static const ToolsClass Tools;

	int dumpTree(TextWriter *out, int flags)
	{
		int type = nodeType();
		if (type <= 0) {
			int id = nodeId();
			if (id == -1 && level() <= 0) {
				nextNode();
			}
		}
		int lev = level();
		return(Tools.dumpTree(out, *this, lev));
	}
};

ARTD_END


#endif // __artd_UnChunker_h


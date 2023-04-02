#include "artd/ChunkerConstants.h"
#include "artd/TextWriter.h"

ARTD_BEGIN

const char ChunkerConstants::nullString[] = "null";
const char ChunkerConstants::UnTaggedStringId[] = "";

const ChunkerConstants::ToolsClass ChunkerConstants::Tools;

static const char spaces[] = "                ";

void 
ChunkerConstants::ToolsClass::appendIndent(TextWriter* w, int indent, int level)
{
	if (indent > 0) {
		int perLevel = level * (indent & 0x07);
		for (;;) {
			if (perLevel >= (int)sizeof(spaces)-1) {
				w->writef(spaces);
				perLevel -= sizeof(spaces)-1;
				continue;
			}
			w->writef("%.*s", perLevel, spaces);
			break;
		}
	}
}

ChunkerConstants::ToolsClass::ToolsClass() 
{
	// typeStrings_ = new char *[0x20];
	// unsigned const char** typeClassIds_ = new unsigned const char *[0x20];

	for (int i = 0; i < ChunkMaxType; ++i) {
		switch (i) {
		case ChunkBoolean:
			//					typeClassIds_[i] = ReservedClassIds.boolClassId;
			tFlags_[i] = fPrimitiveType;
			break;
		case ChunkInt:
			//					typeClassIds_[i] = ReservedClassIds.IntClassId;
			tFlags_[i] = fPrimitiveType | fNumberType;
			break;
		case ChunkUint:
			//					typeClassIds_[i] = ReservedClassIds.UnsignedIntClassId;
			tFlags_[i] = fPrimitiveType | fNumberType;
			break;
		case ChunkReal:
			//					typeClassIds_[i] = ReservedClassIds.DoubleClassId;
			tFlags_[i] = fPrimitiveType | fNumberType;
			break;
		case ChunkCharArray:
			//					typeClassIds_[i] = ReservedClassIds.StringClassId;
			tFlags_[i] = fPrimitiveType | fArrayType;
			break;
		case ChunkIntArray:
			//					typeClassIds_[i] = ReservedClassIds.IntArrayClassId;
			tFlags_[i] = fPrimitiveType | fArrayType;
			break;
		case ChunkUIntArray:
			//					typeClassIds_[i] = ReservedClassIds.UnsignedIntClassId;
			tFlags_[i] = fPrimitiveType | fArrayType;
			break;
		case ChunkRealArray:
			//					typeClassIds_[i] = ReservedClassIds.DoubleArrayClassId;
			tFlags_[i] = fPrimitiveType | fArrayType;
			break;
		case ChunkVCharArray:
			//					typeClassIds_[i] = ReservedClassIds.StringClassId;
			tFlags_[i] = fPrimitiveType | fArrayType | fVArrayType;
			break;
		case ChunkVIntArray:
			//					typeClassIds_[i] = ReservedClassIds.IntArrayClassId;
			tFlags_[i] = fPrimitiveType | fArrayType | fVArrayType;
			break;
		case ChunkVUIntArray:
			//					typeClassIds_[i] = ReservedClassIds.UnsignedIntClassId;
			tFlags_[i] = fPrimitiveType | fArrayType | fVArrayType;
			break;
		case ChunkListNode:
		case ChunkListBegin:
			tFlags_[i] = fListType;
			// typeClassIds_[i] = ReservedClassIds.ObjectArrayClassId;
			break;
		case ChunkNodeBegin:
		case ChunkObjectNode:
		case ChunkNode:
		case ChunkUntaggedNode:
			// typeClassIds_[i] = ReservedClassIds.ObjectClassId;
			tFlags_[i] = fContainerType;
		}
	}

	// Arrays.fill(typeStrings_, "unassigned");

	typeStrings_[ChunkByteArray] = "ByteArray";
	typeStrings_[ChunkBoolean] = "bool";
	typeStrings_[ChunkInt] = "Int";
	typeStrings_[ChunkUint] = "Uint";
	typeStrings_[ChunkReal] = "Real";
	typeStrings_[ChunkCharArray] = "Char[]";
	typeStrings_[ChunkIntArray] = "Int[]";
	typeStrings_[ChunkUIntArray] = "UInt[]";
	typeStrings_[ChunkRealArray] = "Real[]";
	typeStrings_[ChunkString] = "String";
	typeStrings_[ChunkVIntArray] = "VInt[]";
	typeStrings_[ChunkVUIntArray] = "VUInt[]";
	typeStrings_[ChunkClassId] = "ClassId";
	typeStrings_[ChunkExtendedTypeInt] = "Extended";
	typeStrings_[ChunkListNode] = "List";
	typeStrings_[ChunkDelimiter] = "End";
	typeStrings_[ChunkListBegin] = "ListBegin";
	typeStrings_[ChunkNodeBegin] = "NodeBegin";
	typeStrings_[ChunkObjectNode] = "Object";


	typeStrings_[ChunkNode] = "Node";
	typeStrings_[ChunkNode + 1] = "Node+1";
	typeStrings_[ChunkNode + 2] = "Node+2";
	typeStrings_[ChunkNode + 3] = "Node+3";
	typeStrings_[ChunkUntaggedNode] = "Untagged";
	typeStrings_[ChunkUntaggedNode + 1] = "Untagged+1";
	typeStrings_[ChunkUntaggedNode + 2] = "Untagged+2";
	typeStrings_[ChunkUntaggedNode + 3] = "Untagged+3";
	typeStrings_[ChunkPayload] = "Blob";
	typeStrings_[ChunkPayload] = "Blob+1";
	typeStrings_[ChunkPayload] = "Blob+2";
	typeStrings_[ChunkPayload] = "Blob+3";
}


ARTD_END

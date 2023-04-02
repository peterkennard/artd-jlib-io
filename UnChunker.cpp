#include "artd/UnChunker.h"
#include "artd/RcString.h"
#include "artd/HexFormatter.h"

ARTD_BEGIN

const UnChunker::ToolsClass UnChunker::Tools;

int 
UnChunker::ToolsClass::dumpTree(TextWriter *out, UnChunker &uc, int level)
{
	int ret;
	for (;;)
	{
		ret = uc.nextNode();
		if (ret != UnChunker::NtNode) {
            break;
		}
		if (level < 0) {
			level = uc.level();
		}
		appendIndent(out, 2, level);
		RcString name = uc.nodeName();
		if (name) {
			if (name.length() > 0) {
				out->writef(name.c_str());
				//				JsonChunker.appendJsonString(out, name);
				out->writef(": ");
			}
		}
		RcString classString = "null";
		ArtdClassId classId;

		if (uc.nodeClassId(classId) != nullptr) {
			classString = classId.toString();
		}
		const char *typeName = Tools.getTypeName(uc.nodeType());
		if (uc.nodeHasChildren()) {

			const char *delim = "{";
			const char *outDelim = "}";
			if (Tools.isListType(uc.nodeType())) {
				delim = "[";
				outDelim = "]";
			}
			out->writef("%s <id=%d len=%d, type=%s, class=%S>\n", delim, uc.nodeId(), uc.nodeLength(), typeName, classString );
			uc.pushLevel();
			ret = dumpTree(out, uc, level + 1);
			uc.popLevel();
			appendIndent(out, 2, level);
			out->writef("%s[%d]\n", outDelim, level);
			if (ret < 0) {
				break;
			}
		}
		else {

			out->writef("<id=%d, len=%d, class=%S> ", uc.nodeId(), uc.nodeLength(), classString);
			switch (uc.nodeType())
			{
			case UnChunker::ChunkString:
			{
				RcWString buf = uc.readStringNode();
				out->writef("\n");
				appendIndent(out, 2, level + 2);
				out->writef("(%s)\"%S\"\n", typeName, buf);
				break;
			}
			case UnChunker::ChunkInt:
			{
				int anint = (int)uc.readIntNode();
				out->writef("\n");
				appendIndent(out, 2, level + 2);
				out->writef("(%s)%d\n", typeName, anint);
				break;
			}
			case UnChunker::ChunkUint:
			{
				unsigned int anint = (unsigned int)uc.readIntUNode();
				out->writef("\n");
				appendIndent(out, 2, level + 2);
				out->writef("(%s)%u\n", typeName, anint);
				break;
			}
			case UnChunker::ChunkReal:
			{
				double afloat = uc.readFloatNode();
				out->writef("\n");
				appendIndent(out, 2, level + 2);
				out->writef("(%s)%f\n", typeName, afloat);
				break;
			}
			case UnChunker::ChunkByteArray:
			{
				out->writef("\n");
				appendIndent(out, 2, level + 2);
				ByteArray bytes = uc.readByteArrayNode();
				RcString ellipsis = "";
				int bytelen = bytes.length();
				if (bytelen > 20) {
					ellipsis = RcString::format("..[%d]", bytelen);
					bytelen = 20;
				}
				RcString printable = HexFormatter::binToHex(&bytes[0], bytelen);
				out->writef("(%s){%S%S}\n", typeName, printable, ellipsis);
				break;
			}
			default:
				out->writef("\n");
				appendIndent(out, 2, level + 2);
				out->writef("%(%s)?\n", typeName);
				break;
			}
		}
	}
	return(ret);
}




ARTD_END

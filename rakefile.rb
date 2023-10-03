myDir = File.dirname(File.expand_path(__FILE__));
require "#{myDir}/../build-options.rb";

Rakish::CppProject.new(
	:name 		=> "artd-jlib-io",
	:package 	=> "artd",
	:dependsUpon => [
        '../artd-lib-logger',
		'../artd-jlib-base'
	]
) do

	cppDefine('BUILDING_artd_jlib_io');

	addPublicIncludes('include/artd/*.h');

	sourcesList = [];

	if(false && ENV['EMSDK'])
		sourcesList = [
            'Chunker.cpp',
            'JPath.cpp',
            'UnChunker.cpp',
            'ByteArrayInputStream.cpp',
            'ByteArrayOutputStream.cpp',
            'TextReader.cpp',
            'TextWriter.cpp',
            'ChunkerConstants.cpp'
        ];
	else
		sourcesList = [
            'ByteArrayInputStream.cpp',
            'ByteArrayOutputStream.cpp',
            'Chunker.cpp',
            'ChunkerConstants.cpp',
            'FileInputStream.cpp',
            'FileOutputStream.cpp',
            'InputStream.cpp',
            'InputStreamReader.cpp',
            'jlib_io.cpp',
            'JPath.cpp',
            'OutputStream.cpp',
            'OutputStreamWriter.cpp',
            'PosixStreams.cpp',
            'RandomAccessFile.cpp',
            'StdioOutputStream.cpp',
            'StreamsImpl.cpp',
            'StringReader.cpp',
            'TextReader.cpp',
            'TextWriter.cpp',
            'UnChunker.cpp'
        ];
    end

	addSourceFiles(sourcesList);

	setupCppConfig :targetType=>'DLL' do |cfg|
	end

end




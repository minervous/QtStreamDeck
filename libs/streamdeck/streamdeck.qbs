import qbs.FileInfo

DynamicLibrary {
	Depends { name: 'Qt.core' }
	Depends { name: 'Qt.gui' }
	Depends { name: 'QtUsb' }

	name: 'lib-streamdeck'

	cpp.defines: [
		'STREAMDECKLIB_LIBRARY=1',
	]

	cpp.includePaths: [
		'./include/minervous/streamdeck',
		'./src',
	]
	cpp.cxxLanguageVersion: 'c++20'

	Group {
		name: 'Public API'
		prefix: 'include/'
		files: ['**/*.hpp']
	}

	Group {
		name: 'C++ files'
		prefix: 'src/'
		files: ['**/*.cpp', '**/*.hpp']
	}

	Export {
		Depends { name: 'cpp' }
		Depends { name: 'QtUsb' }
		cpp.systemIncludePaths: [
			FileInfo.joinPaths(exportingProduct.sourceDirectory, 'include')
		]
	}
}

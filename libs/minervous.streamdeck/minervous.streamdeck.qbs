// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

import qbs.FileInfo

DynamicLibrary {
	Depends { name: 'bundle' }
	Depends { name: 'Qt.core' }
	Depends { name: 'Qt.gui' }
	Depends { name: 'QtUsb' }

	name: 'lib-minervous.streamdeck'

	cpp.defines: [
		'STREAMDECKLIB_LIBRARY=1',
	]

	cpp.includePaths: [
		'./include/minervous/streamdeck',
		'./src',
	]
	cpp.cxxLanguageVersion: 'c++20'
	cpp.sonamePrefix: qbs.targetOS.contains('macos')? '@rpath' : undefined
	cpp.rpaths: cpp.rpathOrigin

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

	Group {
		fileTagsFilter: qbs.targetOS.contains('macos')? 'bundle.content' : 'dynamiclibrary'
		qbs.install: true
		qbs.installSourceBase: product.buildDirectory
		qbs.installPrefix: project.installContentsPath
		qbs.installDir: project.installLibraryDir
	}

	Export {
		Depends { name: 'cpp' }
		Depends { name: 'QtUsb' }
		cpp.systemIncludePaths: [
			FileInfo.joinPaths(exportingProduct.sourceDirectory, 'include')
		]
	}
}

// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

import qbs
import qbs.FileInfo

DynamicLibrary {
	Depends { name: 'bundle' }
	Depends { name: 'easy.qmldir' }
	Depends { name: 'Qt.qml' }
	Depends { name: 'Qt.qmlmodels-private' }
	Depends { name: 'Qt.quick' }

	Depends { name: 'lib-minervous.streamdeck' }

	name: 'ext-minervous.streamdeck'

	property string moduleName: 'minervous.streamdeck'
	property string moduleVersion: '1.0'
	readonly property string modulePath: moduleName.replace(/\./g, '/')

	cpp.includePaths: ['./src']

	cpp.sonamePrefix: qbs.targetOS.contains('macos')? '@rpath' : undefined
	cpp.rpaths: {
		var installDirAbsPath = FileInfo.joinPaths(qbs.installRoot, qbs.installPrefix, installDir)
		var libraryDirAbsPath = FileInfo.joinPaths(qbs.installRoot, qbs.installPrefix, project.installLibraryDir)

		return FileInfo.joinPaths(
			cpp.rpathOrigin,
			FileInfo.relativePath(installDirAbsPath, libraryDirAbsPath)
		)
	}

	easy.qmldir.classname: 'minervous::streamdeck::qml::Plugin'
	easy.qmldir.prefer: ':/qt/qml/' + modulePath + '/'

	Qt.qml.importName: moduleName
	Qt.qml.importVersion: moduleVersion
	Qt.qml.typesInstallDir: installDir

	bundle.isBundle: false
	install: true
	installDir: FileInfo.joinPaths(
		project.installImportsDir,
		modulePath
	)
	qbs.installPrefix: project.installContentsPath

	Group {
		name: 'C++ files'
		prefix: 'src/'
		files: ['**/*.cpp', '**/*.hpp']
	}

	Group {
		name: 'QML files'
		files: ['**']
		prefix: 'qml/'
		Qt.core.resourceSourceBase: 'qml/'
		Qt.core.resourcePrefix: '/qt/qml/'
		fileTags: ['qt.core.resource_data', 'easy.qmldir.inputs']
	}

	EasyQmldir {
		qbs.install: true
		qbs.installDir: product.installDir
	}

	Export {
		Depends { name: 'lib-minervous.streamdeck' }
	}
}

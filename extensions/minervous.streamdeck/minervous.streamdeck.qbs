import qbs
import qbs.FileInfo

DynamicLibrary {
	Depends { name: 'Qt.qml' }
	Depends { name: 'easy.qmldir' }

	Depends { name: 'lib-streamdeck' }

	name: 'ext-minervous-streamdeck'

	property string moduleName: 'minervous.streamdeck'
	property string moduleVersion: '1.0'
	readonly property string modulePath: moduleName.replace(/\./g, '/')

	cpp.includePaths: ['./src']

	easy.qmldir.classname: 'minervous::streamdeck::StreamDeckPlugin'
	easy.qmldir.prefer: ':/qt/qml/' + modulePath + '/'

	Properties {
		condition: qbs.targetOS.contains('linux')
		cpp.rpaths: {
			var installDirAbsPath = FileInfo.joinPaths(qbs.installRoot, qbs.installPrefix, installDir)
			var libraryDirAbsPath = FileInfo.joinPaths(qbs.installRoot, qbs.installPrefix, project.installLibraryDir)

			return FileInfo.joinPaths(
				cpp.rpathOrigin,
				FileInfo.relativePath(installDirAbsPath, libraryDirAbsPath)
			)
		}
	}

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
}

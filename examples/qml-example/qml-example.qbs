CppApplication {
	Depends { name: 'Qt.core' }
	Depends { name: 'Qt.gui' }
	Depends { name: 'Qt.qml' }
	Depends { name: 'Qt.quick' }
	Depends { name: 'ext-minervous-streamdeck' }

	name: 'stream-deck-qml-example'

	builtByDefault: false

	Qt.qml.importName: 'minervous.streamdeck.example'
	Qt.qml.importVersion: '1.0'

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
		fileTags: ['qt.core.resource_data']
	}
}

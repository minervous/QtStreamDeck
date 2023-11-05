CppApplication {
	Depends { name: 'Qt.core' }
	Depends { name: 'Qt.gui' }
	Depends { name: 'lib-streamdeck' }

	name: 'stream-deck-cpp-example'

	builtByDefault: false

	Group {
		name: 'C++ files'
		prefix: 'src/'
		files: ['**/*.cpp', '**/*.hpp']
	}

	Group {
		name: 'Assets'
		files: ['**']
		prefix: 'images/'
		Qt.core.resourceSourceBase: product.sourceDirectory
		fileTags: ['qt.core.resource_data']
	}
}

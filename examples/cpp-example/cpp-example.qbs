CppApplication {
    Depends { name: 'Qt.core' }
    Depends { name: 'Qt.gui' }
    Depends { name: 'aux-example-images' }
    Depends { name: 'lib-streamdeck' }

    name: 'stream-deck-cpp-example'

    builtByDefault: false

    Group {
        name: 'C++ files'
        prefix: 'src/'
        files: ['**/*.cpp', '**/*.hpp']
    }
}

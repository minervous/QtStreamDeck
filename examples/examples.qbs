Project {
    references: [
        'cpp-example/cpp-example.qbs',
        'qml-example/qml-example.qbs',
    ]

    DynamicLibrary {
        Depends { name: 'Qt.core' }

        name: 'aux-example-images'

        ResourceGroup {
            prefix: 'images/'
            files: '*.png'
        }
    }
}

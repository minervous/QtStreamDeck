Project {
	references: [
		'cpp-example/cpp-example.qbs',
		'qml-example/qml-example.qbs',
	]

	StaticLibrary {
		Depends { name: 'Qt.core' }

		name: 'aux-example-images'

		ResourceGroup {
			prefix: 'images/'
			files: '*.png'
		}
	}
}

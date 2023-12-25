Project {
	references: [
		'cpp-example/cpp-example.qbs',
		'qml-example/qml-example.qbs',
	]

	StaticLibrary {
		Depends { name: 'Qt.core' }

		name: 'aux-example-images'

		Group {
			prefix: 'images/'
			files: '*.png'
			Qt.core.resourcePrefix: '/examples/images/'
			fileTags: ['qt.core.resource_data']
		}
	}
}

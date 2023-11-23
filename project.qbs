Project {
	property bool withExternalDeps: true

	qbsSearchPaths: [
		'qbs',
		'3rdparty/easy.qmldir.qbs',
	]

	references: [
		'3rdparty/3rdparty.qbs',
		'examples/examples.qbs',
		'extensions/extensions.qbs',
		'libs/libs.qbs',
	]
}

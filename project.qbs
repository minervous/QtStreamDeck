// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

Project {
	property string installContentsPath
	property string installLibraryDir

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

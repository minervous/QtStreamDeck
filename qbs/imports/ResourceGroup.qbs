import qbs.FileInfo

Group {
	property path resourceSourceBase: sourceDirectory
	property string resourcePrefix: FileInfo.joinPaths(
		'/', FileInfo.relativePath(project.sourceDirectory, sourceDirectory)
	)

	files: ['**']
	Qt.core.resourceSourceBase: resourceSourceBase
	Qt.core.resourcePrefix: resourcePrefix
	fileTags: ['qt.core.resource_data']
}

import qbs.FileInfo
import qbs.Probes

Project {
	readonly property string $path: path

	SubProject {
		filePath: qtUsbProbe.filePath
		condition: qtUsbProbe.found && project.withExternalDeps
		Properties {
			installContentsPath: parent.installContentsPath
			installLibraryDir: parent.installLibraryDir
		}
	}

	Probes.PathProbe {
		id: qtUsbProbe
		searchPaths: FileInfo.joinPaths($path, 'QtUsb')
		names: 'project.qbs'
	}
}

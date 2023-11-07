import qbs.Environment

Module {
	setupRunEnvironment: {
		Environment.putEnv('QML_IMPORT_PATH', product.qbs.installRoot)
	}
}

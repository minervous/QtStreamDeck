// Copyright (c) Minervous
// SPDX-License-Identifier: BUSL-1.1

import qbs.Environment

Module {
	setupRunEnvironment: {
		Environment.putEnv('QML_IMPORT_PATH', product.qbs.installRoot)
	}
}

Project {
    qbsSearchPaths: [
        'qbs',
        '3rdparty/easy.qmldir.qbs',
    ]

    references: [
        'examples/examples.qbs',
        'extensions/extensions.qbs',
        'libs/libs.qbs',
        '3rdparty/QtUsb/project.qbs'
    ]
}

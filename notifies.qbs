import qbs

CppApplication {
	name:"notifier"

	Depends {
		name: "Qt"
		submodules: ["core", "network"]
	}

	files: [
		"main.cpp"
	]

	Group {
		name: "confi"
		files: "config.json"
		qbs.install: true
		qbs.installDir: "app"
	}
	
	Group {
        fileTagsFilter: "application"
        qbs.install: true
        qbs.installDir: "app"
    }
}
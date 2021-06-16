pipeline {
	agent any 
	stages {
		stage ('install prerequisites') {
			steps {
				sh 'sudo apt-get install git'
				sh 'sudo apt-get install cmake'
				sh 'sudo apt-get install g++'
			}
		}

		stage('Generate projects') {
			steps {
				sh 'cmake .'
			}
		}
		stage('Build') {
			steps {
				sh 'cmake --build .'
			}
		}
		stage('Tst') {
			steps {
				sh 'cd bin/Debug'
				sh './Editor'
			}
		}
	}
}
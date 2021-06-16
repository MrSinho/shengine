pipeline {
	agent any 
	stages {
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
		stage('Test') {
			steps {
				sh 'cd bin/Debug'
				sh './Editor'
			}
		}
	}
}
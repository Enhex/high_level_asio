from conans import ConanFile, tools
import os


class AsioutilityConan(ConanFile):
	name = "asio-utility"
	version = "master"
	description = "High level utility functionality for Asio library"
	no_copy_source = True
	requires = "Asio/master@enhex/stable"
	exports_sources = "include/*"

	def package(self):
		self.copy("*.h")

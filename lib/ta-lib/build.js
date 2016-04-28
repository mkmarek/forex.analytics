var exec = require('child_process').exec;
var msbuildPath = '"C:/Program Files (x86)/MSBuild/14.0/Bin/';

console.log('building talib functions...');
var flags = "";

if(process.platform == 'win32') {
  process.chdir('./lib/ta-lib/make/csr/windows/msbuild/');

  exec(msbuildPath + 'MSBuild" ./ta_lib.sln /property:Configuration=csr /property:Platform=x64', function(err, stdout, stderr) {
    console.log(stdout, stderr);
	});
} else {
  var flags = "";

  if (process.platform == 'darwin') {
  	var arch = process.arch == 'ia32' ? 'i386' : process.arch == 'x64' ? 'x86_64' : process.arch;
  	flags = 'export CFLAGS="-arch ' + arch + '" && export LDFLAGS="-arch ' + arch + '" && ';
  }

  process.chdir('./lib/ta-lib/make/csr/linux/g++/');
  exec(flags + 'make');
}

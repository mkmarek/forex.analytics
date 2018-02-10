var exec = require('child_process').exec;

console.log('building talib functions...');
var flags = "";

if(process.platform == 'win32') {
  function startMsbuild(version, argMsbuildPath) {
    process.chdir('./lib/ta-lib/make/csr/windows/msbuild' + version + '/');

    exec('"' + argMsbuildPath + '\\MSBuild\\' + version + '.0\\Bin\\MSBuild" .\\ta_lib.sln /property:Configuration=csr /property:Platform=x64', function(err, stdout, stderr) {
      console.log(stdout, stderr);
    });
  }

  var fs = require('fs');
  var msbuildPath = process.env['ProgramFiles(x86)'];

  if (!fs.existsSync(msbuildPath + '\\MSBuild\\14.0\\Bin')) {
    var vswhere = exec('"' + __dirname + '\\vswhere.exe" -latest -products * -requires Microsoft.Component.MSBuild -property installationPath', function(err, stdout, stderr) {
      if (err instanceof Error)
        throw err;
    });

    vswhere.stdout.on('data', function(data) {
      startMsbuild(15, data.trim());
    });
  }
  else {
    startMsbuild(14, msbuildPath);
  }
} else {
  var flags = "";

  if (process.platform == 'darwin') {
  	var arch = process.arch == 'ia32' ? 'i386' : process.arch == 'x64' ? 'x86_64' : process.arch;
  	flags = 'export CFLAGS="-arch ' + arch + '" && export LDFLAGS="-arch ' + arch + '" && ';
  }

  process.chdir('./lib/ta-lib/make/csr/linux/g++/');
  exec(flags + 'make');
}

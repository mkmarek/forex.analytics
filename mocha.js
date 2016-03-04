var Mocha = require('mocha');
var fs = require('fs');
var path = require('path');
var read = require('fs-readdir-recursive')

var mocha = new Mocha({
    reporter: 'list'
});

var testDir = 'tests/'

// Add each .js file to the mocha instance
read(testDir).filter(function(file){
    // Only keep the .js files
    return file.substr(-3) === '.js';

}).forEach(function(file){
    mocha.addFile(
        path.join(testDir, file)
    );
});

// Run the tests.
mocha.run(function(failures){
  process.on('exit', function () {
    process.exit(failures);  // exit with non-zero status if there were failures
  });
});

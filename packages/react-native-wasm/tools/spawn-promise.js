const { spawn } = require('child_process');

module.exports = (command, args, ...rest) =>
  new Promise((resolve, reject) => {
    console.log('Running', command, args.join(' '));

    const child = spawn(command, args, ...rest);

    child.on('exit', () => {
      resolve();
    });

    child.stdout.on('data', (data) => {
      console.log(`${data}`);
    });

    child.stderr.on('data', (data) => {
      console.error(`${data}`);
    });
  });

const fs = require('fs/promises');
const crypto = require('crypto');

function generateChecksum(data) {
  return crypto.createHash('md5').update(data, 'utf8').digest('hex');
}

async function generateFileChecksum(file) {
  return generateChecksum(await fs.readFile(file));
}

module.exports = { generateFileChecksum, generateChecksum };

const Koa = require('koa');
const koaStatic = require('koa-static');
const path = require('path');

module.exports = ({ reactNativeWasmDir }) => {
  const app = new Koa();

  app.use(async (ctx, next) => {
    ctx.set('Access-Control-Allow-Origin', '*');
    ctx.set('Access-Control-Allow-Headers', 'Origin, X-Requested-With, Content-Type, Accept');
    ctx.set('Access-Control-Allow-Methods', 'GET, HEAD, OPTIONS');
    ctx.set('Cross-Origin-Opener-Policy', 'same-origin');
    ctx.set('Cross-Origin-Embedder-Policy', 'require-corp');

    return next();
  });

  app.use(koaStatic(path.join(reactNativeWasmDir, 'dist')));

  app.listen(8081, () => {
    console.log('Serving files through 8081');
  });
};

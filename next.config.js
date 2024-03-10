/**
 * @type {import('next').NextConfig}
 */
const path = require('path')

const nextConfig = {
    output: 'export',
    assetPrefix: 'https://k1n0r4.github.io/',
    sassOptions: {
        includePaths: [path.join(__dirname, 'styles')],
    },
    images: {
        unoptimized: true,
    },
}

module.exports = nextConfig

/**
 * @type {import('next').NextConfig}
 */
const path = require('path')

let assetPrefix = 'https://k1n0r4.github.io/'
if (process.env.NODE_ENV === 'development') {
    assetPrefix = ''
}

const nextConfig = {
    output: 'export',
    assetPrefix,
    sassOptions: {
        includePaths: [path.join(__dirname, 'styles')],
    },
    images: {
        unoptimized: true,
    },
}

module.exports = nextConfig

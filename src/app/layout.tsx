import type { Metadata } from 'next';

import './globals.css';
import { SITE_CONFIG } from '../../config';

export const metadata: Metadata = {
    title: SITE_CONFIG.title,
    description: SITE_CONFIG.description,
    openGraph: {
        images: [SITE_CONFIG.coverImage],
    },
};

export default function RootLayout({
    children,
}: Readonly<{
    children: React.ReactNode;
}>) {
    return (
        <html lang="en">
            <head>
                <link rel="apple-touch-icon" sizes="180x180" href="/favicon/apple-touch-icon.png" />
                <link rel="icon" type="image/png" sizes="32x32" href="/favicon/favicon-32x32.png" />
                <link rel="icon" type="image/png" sizes="16x16" href="/favicon/favicon-16x16.png" />
                <link rel="manifest" href="/favicon/site.webmanifest" />
                <link rel="mask-icon" href="/favicon/safari-pinned-tab.svg" color="#000000" />
                <link rel="shortcut icon" href="/favicon/favicon.ico" />
                <meta name="msapplication-TileColor" content="#000000" />
                <meta name="msapplication-config" content="/favicon/browserconfig.xml" />
                <meta name="theme-color" content="#000" />
                <meta charSet="utf-8" />
                <meta name="viewport" content="width=device-width, initial-scale=1.0" />
                <meta httpEquiv="X-UA-Compatible" content="IE=edge" />
                <meta name="HandheldFriendly" content="True" />
                <meta name="MobileOptimized" content="320" />
                <meta name="apple-mobile-web-app-capable" content="yes" />
                <meta name="apple-mobile-web-app-status-bar-style" content="black-translucent" />

                <meta property="og:site_name" content={SITE_CONFIG.title} />
                <meta property="og:title" content={SITE_CONFIG.title} />
                <meta property="og:description" content={SITE_CONFIG.description} />
                <meta property="og:image" content={process.env.NEXT_PUBLIC_DOMAIN + SITE_CONFIG.coverImage} />
                <meta property="og:url" content={process.env.NEXT_PUBLIC_DOMAIN} />
                <meta property="og:type" content="blog" />

                <link rel="shortcut icon" href="/assets/images/favicon.png" type="image/png" />
                <link
                    rel="stylesheet"
                    href="//brick.a.ssl.fastly.net/Linux+Libertine:400,400i,700,700i/Open+Sans:400,400i,700,700i"
                />

                <link rel="stylesheet" type="text/css" media="screen" href="/main.css" />
                <link rel="stylesheet" type="text/css" media="print" href="/print.css" />
                <link
                    rel="stylesheet"
                    type="text/css"
                    media="screen"
                    href="https://cdn.jsdelivr.net/gh/hung1001/font-awesome-pro@4cac1a6/css/all.css"
                />
                <link
                    rel="stylesheet"
                    href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.5.1/css/brands.min.css"
                    integrity="sha512-8RxmFOVaKQe/xtg6lbscU9DU0IRhURWEuiI0tXevv+lXbAHfkpamD4VKFQRto9WgfOJDwOZ74c/s9Yesv3VvIQ=="
                    crossOrigin="anonymous"
                    referrerPolicy="no-referrer"
                />
            </head>
            <body>
                {children}
            </body>
        </html>
    );
}

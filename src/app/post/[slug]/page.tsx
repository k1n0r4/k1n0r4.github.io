import { Metadata } from 'next';
import { notFound } from 'next/navigation';
import Link from 'next/link';
import Script from 'next/script';
import Image from 'next/image';

import { SITE_CONFIG } from '../../../../config';

import { getAllPosts, getPostBySlug } from '@/lib/api';
import markdownToHtml from '@/lib/markdownToHtml';

export default async function Post({ params }: Params) {
    const post = getPostBySlug(params.slug);
    if(!post) return notFound();

    const content = await markdownToHtml(post.content || '');

    return (
        <>
            <main className="content" role="main">
                <Link href="/" className="logo-readium">
                    <span className="logo" style={{ backgroundImage: `url(${SITE_CONFIG.logo})` }}></span>
                </Link>
                <article className="post">
                    {post?.coverImage ? (
                        <div className="article-image h-[100dvh] w-screen">
                            <div id="article-cover" className="w-screen h-screen">
                                <Image
                                    className="w-full h-full object-cover object-center"
                                    src={post.coverImage}
                                    alt={post.title}
                                    draggable={false}
                                    width={1800}
                                    height={1200}
                                />
                            </div>
                            <div className="post-meta">
                                <h1 className="post-title">{post.title}</h1>
                                <div className="cf post-meta-text">
                                    <div className="author-image" style={{ backgroundImage: `url(${SITE_CONFIG.author.avatarURL || SITE_CONFIG.logo})` }}>Blog Logo</div>
                                    <h4 className="author-name font-bold">{SITE_CONFIG.author.name}</h4>
                                    &nbsp;
                                    on
                                    &nbsp;
                                    <span>{post.date}</span>
                                    {/*<time datetime={post.date}>{post.date}</time>*/}
                                </div>
                                <div className="text-center">
                                    <a href="#topofpage" className="topofpage"><i className="fa fa-angle-down"></i></a>
                                </div>
                            </div>
                        </div>
                    ) : (
                        <div className="noarticleimage">
                            <div className="post-meta">
                                <h1 className="post-title">{post.title}</h1>
                                <div className="cf post-meta-text">
                                    <div
                                        className="author-image"
                                        style={{ backgroundImage: `url(${SITE_CONFIG.author.avatarURL || SITE_CONFIG.logo})` }}
                                    >
                                        Blog
                                        Logo
                                    </div>
                                    <h4 className="author-name font-bold">{SITE_CONFIG.author.name}</h4>
                                    &nbsp;
                                    on
                                    &nbsp;
                                    <span>{post.date}</span>
                                    {/*<time datetime={post.date}>{post.date}</time>*/}
                                </div>
                            </div>
                        </div>
                    )}
                    <br />
                    <br />
                    <br />
                    <div className="max-w-[800px] mx-auto px-4">
                        <section className="post-content z-20">
                            <div className="post-reading hidden">
                                <span className="post-reading-time">
                                    {post.content.trim().split(/\s+/).length / 200}
                                    {' '}
                                    min read &nbsp; &nbsp; | &nbsp; &nbsp;
                                    {post.content.trim().split(/\s+/).length}
                                    {' '}
                                    words
                                </span>
                            </div>
                            <a id="topofpage" />
                            <div className="content" dangerouslySetInnerHTML={{ __html: content }}></div>
                        </section>
                        <footer className="post-footer">
                            <section className="share">
                                <Link href={`https://twitter.com/share?text=Hey, I just read ${post.title} on ${SITE_CONFIG.title}!&url=${process.env.NEXT_PUBLIC_DOMAIN}/post/${post.slug}`} target="_blank" className="share-twitter">
                                    Share on Twitter
                                </Link>
                            </section>
                        </footer>
                        <div className="bottom-teaser cf">
                            <div className="isLeft">
                                <h5 className="index-headline featured"><span>Written by</span></h5>
                                <section className="author">
                                    <div
                                        className="author-image"
                                        style={{ backgroundImage: `url(${SITE_CONFIG.author.avatarURL || '/assets/images/logo.jpg'})` }}
                                    >
                                        Blog
                                        Logo
                                    </div>
                                    <h4>{SITE_CONFIG.author.name}</h4>
                                    <p className="bio">{SITE_CONFIG.author.bio}</p>
                                    <hr />
                                    <p className="published">
                                        Published
                                        {' '}
                                        {post.date}
                                    </p>
                                </section>
                                <section className="published site-footer opacity-30 text-xs text-center">
                                    All content copyright &nbsp;
                                    <a href={'mailto:' + SITE_CONFIG.author.mail}>{SITE_CONFIG.author.name}</a>
                                    {' '}
                                    &copy;
                                    {new Date().getFullYear()}
                                    {' '}
                                &nbsp; · &nbsp;All rights reserved.
                                </section>
                            </div>
                        </div>
                    </div>
                </article>
            </main>
            <div className="bottom-closer">
                <div
                    className="background-closer-image"
                    style={{ backgroundImage: `url(${SITE_CONFIG.coverImage})` }}
                >
                    Image
                </div>
                <div className="inner">
                    <h1 className="blog-title">{SITE_CONFIG.title}</h1>
                    <h2 className="blog-description">{SITE_CONFIG.description}</h2>
                    <Link href="/" className="btn">Back to Overview</Link>
                </div>
            </div>
            <Script id="custom-transition-js">
                {`window.addEventListener('scroll', function() {
                    const image = document.getElementById('article-cover');
                    if (image) {
                        const scrollTop = window.scrollY || window.pageYOffset;
                        const opacity = 1 - Math.max(scrollTop / 700, 0); // Adjust 700 according to your needs
                        const translateY = scrollTop / 3; // Adjust 3 according to your needs
                        image.style.opacity = opacity;
                        image.style.transform = 'translateY(' + translateY + 'px)';
                    }
                });`}
            </Script>
        </>
    );
}

type Params = {
    params: {
        slug: string;
    };
};

export function generateMetadata({ params }: Params): Metadata {
    const post = getPostBySlug(params.slug);

    if(!post) {
        return notFound();
    }

    const title = `${post.title} | ${SITE_CONFIG.title}`;

    return {
        openGraph: {
            title,
            images: [post.coverImage || SITE_CONFIG.coverImage],
        },
    };
}

export async function generateStaticParams() {
    const posts = getAllPosts();

    return posts.map((post) => ({
        slug: post.slug,
    }));
}

import Link from 'next/link';

import { Post } from '@/interfaces/post';

const ArticlePreview = ({ post }: { post: Post }) => {
    return (
        <article className="post pb-4" role="article">
            <div className="article-item">
                <header className="post-header">
                    <h2 className="post-title" itemProp="name">
                        <Link href={`/post/${post.slug}`} itemProp="url">
                            {post.title}
                        </Link>
                    </h2>
                </header>
                <section className="post-excerpt mt-3 !text-sm opacity-60" itemProp="description">
                    <p>
                        {post.description || post.excerpt}
                    </p>
                </section>
                <div className="post-meta">
                    <time dateTime="30 November 2014">
                        {post.date}
                    </time>
                </div>
            </div>
        </article>
    );
};

export default ArticlePreview;
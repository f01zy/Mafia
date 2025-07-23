export class Variables {
  public static SMTP_HOST: string;
  public static SMTP_PORT: string;
  public static SMTP_USERNAME: string;
  public static SMTP_PASSWORD: string;

  public static JWT_ACCESS_SECRET: string;
  public static JWT_REFRESH_SECRET: string;

  public static DATABASE_URL: string;
  public static PORT: string;

  public static initialize() {
    this.SMTP_HOST = process.env.SMTP_HOST!;
    this.SMTP_PORT = process.env.SMTP_PORT!;
    this.SMTP_USERNAME = process.env.SMTP_USERNAME!;
    this.SMTP_PASSWORD = process.env.SMTP_PASSWORD!;

    this.JWT_ACCESS_SECRET = process.env.JWT_ACCESS_SECRET!;
    this.JWT_REFRESH_SECRET = process.env.JWT_REFRESH_SECRET!;

    this.DATABASE_URL = process.env.DATABASE_URL!;
    this.PORT = process.env.PORT!;
  }
}

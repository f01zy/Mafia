import { Variables } from "../env/variables.env"
import nodemailer from "nodemailer"
import { ApiError } from "../exceptions/api.exception"

export class MailService {
  private transporter: nodemailer.Transporter

  constructor() {
    this.transporter = nodemailer.createTransport({
      host: Variables.SMTP_HOST,
      port: Variables.SMTP_PORT,
      secure: false,
      auth: {
        user: Variables.SMTP_USERNAME,
        pass: Variables.SMTP_PASSWORD
      }
    } as nodemailer.TransportOptions)
  }

  public async sendActivationMail(to: string, code: string) {
    try {
      await this.transporter.sendMail({
        from: process.env.SMTP_USER,
        to,
        subject: `Activation account on Mafia.`,
        text: "",
        html:
          `
          <div>
            <h1>For account activation enter this code.</h1>
            <p>${code}</p>
          </div>
          `
      })
    } catch (err) {
      throw ApiError.BadRequest("Cannot send email.")
    }
  }
}
